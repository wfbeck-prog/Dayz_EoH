const fs = require('fs');
const path = require('path');
const http = require('http');
const https = require('https');

function loadEnvFile(filePath) {
  if (!fs.existsSync(filePath)) return;
  const lines = fs.readFileSync(filePath, 'utf8').split(/\r?\n/);
  for (const line of lines) {
    const trimmed = line.trim();
    if (!trimmed || trimmed.startsWith('#')) continue;
    const eq = trimmed.indexOf('=');
    if (eq === -1) continue;
    const key = trimmed.slice(0, eq).trim();
    const value = trimmed.slice(eq + 1).trim();
    if (!process.env[key]) process.env[key] = value;
  }
}

loadEnvFile(path.join(__dirname, '.env'));

const LOG_DIR = process.env.LOG_DIR || '';
const DISCORD_WEBHOOK_URL = process.env.DISCORD_WEBHOOK_URL || '';
const RADIO_WEBHOOK_URL = process.env.RADIO_WEBHOOK_URL || '';
const POLL_INTERVAL_MS = Number(process.env.POLL_INTERVAL_MS || 5000);
const STATE_FILE = process.env.STATE_FILE || path.join(__dirname, 'airdrop-log-state.json');
const SCAN_OLD_LINES_ON_FIRST_RUN = String(process.env.SCAN_OLD_LINES_ON_FIRST_RUN || '0') === '1';

if (!LOG_DIR) {
  console.error('[EoH Airdrop Watcher] LOG_DIR is missing. Copy .env.example to .env and set LOG_DIR.');
  process.exit(1);
}

if (!DISCORD_WEBHOOK_URL) {
  console.error('[EoH Airdrop Watcher] DISCORD_WEBHOOK_URL is missing.');
  process.exit(1);
}

function readState() {
  try {
    if (fs.existsSync(STATE_FILE)) return JSON.parse(fs.readFileSync(STATE_FILE, 'utf8'));
  } catch (err) {
    console.warn('[EoH Airdrop Watcher] Could not read state file:', err.message);
  }
  return { files: {}, sent: {} };
}

function writeState(state) {
  try {
    fs.writeFileSync(STATE_FILE, JSON.stringify(state, null, 2));
  } catch (err) {
    console.warn('[EoH Airdrop Watcher] Could not write state file:', err.message);
  }
}

function newestLogFiles(dir) {
  if (!fs.existsSync(dir)) return [];
  return fs.readdirSync(dir)
    .filter(name => name.toLowerCase().endsWith('.log'))
    .map(name => {
      const full = path.join(dir, name);
      const stat = fs.statSync(full);
      return { name, full, mtimeMs: stat.mtimeMs, size: stat.size };
    })
    .sort((a, b) => b.mtimeMs - a.mtimeMs)
    .slice(0, 8);
}

function parseAirdropLine(line) {
  if (!line.includes('[MissionAirdrop]')) return null;

  const timeMatch = line.match(/^(\d{2}:\d{2}:\d{2}\.\d{3})/);
  const locationMatch = line.match(/heading towards\s+"([^"]+)"/i);
  const posMatch = line.match(/pos=<([^>]+)>/i);
  const typeMatch = line.match(/type=([^\s\)]+)\)?/i);
  const containerMatch = line.match(/with a\s+([^\s]+)/i);

  const rawRegion = locationMatch ? locationMatch[1] : 'Unknown Region';
  const missionType = typeMatch ? typeMatch[1] : 'Unknown';
  const container = containerMatch ? containerMatch[1] : 'Unknown Container';
  const pos = posMatch ? posMatch[1] : '';
  const category = resolveCategory(missionType, rawRegion, container);
  const region = cleanRegion(rawRegion, category);
  const threat = resolveThreat(category, region);

  return {
    time: timeMatch ? timeMatch[1] : '',
    rawRegion,
    region,
    missionType,
    container,
    pos,
    category,
    threat,
    raw: line
  };
}

function cleanRegion(region, category) {
  let clean = region || 'Unknown Region';
  const prefixes = ['Drugs ', 'Medical ', 'BaseBuilding ', 'Base Building ', 'Supply '];
  for (const prefix of prefixes) {
    if (clean.toLowerCase().startsWith(prefix.toLowerCase())) {
      clean = clean.slice(prefix.length);
      break;
    }
  }
  return clean.replace(/-/g, ' ').replace(/\s+/g, ' ').trim() || region;
}

function resolveCategory(missionType, region, container) {
  const source = `${missionType} ${region} ${container}`.toLowerCase();
  if (source.includes('drug')) return 'Drugs';
  if (source.includes('medical') || source.includes('medic')) return 'Medical';
  if (source.includes('basebuilding') || source.includes('base building') || source.includes('building')) return 'Base Building';
  return 'Supply';
}

function resolveThreat(category, region) {
  const r = region.toLowerCase();
  if (category === 'Drugs') return 'extreme';
  if (r.includes('nwaf') || r.includes('tisy') || r.includes('balota') || r.includes('neaf')) return 'extreme';
  return 'high';
}

function isHighValue(event) {
  return event.category === 'Drugs' || event.threat === 'extreme';
}

function buildDiscordMessage(event) {
  const flavor = {
    Drugs: 'Smuggler-grade cargo has entered the relay net. Expect armed contact and black-market movement.',
    Medical: 'Emergency medical cargo is in the air. Terje field supplies may be present.',
    'Base Building': 'Construction-grade supplies have been detected near a contested route. Territory groups may move fast.',
    Supply: 'A military supply beacon has been detected by the old relay network.'
  }[event.category] || 'A military supply beacon has been detected by the old relay network.';

  let msg = '📦 AIRDROP SIGNAL DETECTED\n\n';
  msg += `Category: ${event.category}\n`;
  msg += `Region: ${event.region}\n`;
  msg += 'Status: inbound\n';
  msg += `Threat level: ${event.threat}\n`;
  if (event.time) msg += `Log time: ${event.time}\n`;
  if (event.missionType && event.missionType !== 'Unknown') msg += `Mission: ${event.missionType}\n`;
  if (event.container && event.container !== 'Unknown Container') msg += `Container: ${event.container}\n`;
  if (event.pos) msg += `Position: ${event.pos}\n`;
  msg += `\n${flavor}`;
  return msg;
}

function postWebhook(url, content) {
  return new Promise((resolve, reject) => {
    const body = JSON.stringify({ content });
    const parsed = new URL(url);
    const client = parsed.protocol === 'http:' ? http : https;
    const req = client.request({
      method: 'POST',
      hostname: parsed.hostname,
      path: parsed.pathname + parsed.search,
      headers: {
        'Content-Type': 'application/json',
        'Content-Length': Buffer.byteLength(body)
      }
    }, res => {
      res.resume();
      res.on('end', () => {
        if (res.statusCode >= 200 && res.statusCode < 300) resolve();
        else reject(new Error(`Discord returned HTTP ${res.statusCode}`));
      });
    });
    req.on('error', reject);
    req.write(body);
    req.end();
  });
}

async function processLine(state, line) {
  const event = parseAirdropLine(line);
  if (!event) return;

  const key = `${event.time}|${event.missionType}|${event.rawRegion}|${event.container}`;
  if (state.sent[key]) return;

  const message = buildDiscordMessage(event);
  await postWebhook(DISCORD_WEBHOOK_URL, message);
  console.log(`[EoH Airdrop Watcher] Sent airdrop alert: ${key}`);

  if (RADIO_WEBHOOK_URL && isHighValue(event)) {
    await postWebhook(RADIO_WEBHOOK_URL, message);
    console.log(`[EoH Airdrop Watcher] Sent radio alert: ${key}`);
  }

  state.sent[key] = Date.now();
}

async function scan() {
  const state = readState();
  const files = newestLogFiles(LOG_DIR);

  for (const file of files) {
    const hasSeenFile = Object.prototype.hasOwnProperty.call(state.files, file.full);
    if (!hasSeenFile && !SCAN_OLD_LINES_ON_FIRST_RUN) {
      state.files[file.full] = file.size;
      continue;
    }

    const previousSize = state.files[file.full] || 0;
    if (file.size < previousSize) state.files[file.full] = 0;
    const start = state.files[file.full] || 0;
    if (file.size === start) continue;

    const fd = fs.openSync(file.full, 'r');
    const length = file.size - start;
    const buffer = Buffer.alloc(length);
    fs.readSync(fd, buffer, 0, length, start);
    fs.closeSync(fd);

    const text = buffer.toString('utf8');
    const lines = text.split(/\r?\n/);
    for (const line of lines) {
      try {
        await processLine(state, line);
      } catch (err) {
        console.warn('[EoH Airdrop Watcher] Failed to process line:', err.message);
      }
    }

    state.files[file.full] = file.size;
  }

  writeState(state);
}

console.log('[EoH Airdrop Watcher] Watching:', LOG_DIR);
console.log('[EoH Airdrop Watcher] Poll interval:', POLL_INTERVAL_MS, 'ms');
console.log('[EoH Airdrop Watcher] Scan old lines on first run:', SCAN_OLD_LINES_ON_FIRST_RUN ? 'yes' : 'no');

scan().catch(err => console.error('[EoH Airdrop Watcher] Scan failed:', err.message));
setInterval(() => {
  scan().catch(err => console.error('[EoH Airdrop Watcher] Scan failed:', err.message));
}, POLL_INTERVAL_MS);
