require('dotenv').config();

const fs = require('fs');
const chokidar = require('chokidar');
const OpenAI = require('openai');

const logFile = process.env.EOH_EVENTS_FILE || './events.log';
const webhook = process.env.DISCORD_WEBHOOK_URL;

const client = new OpenAI({
    apiKey: process.env.OPENAI_API_KEY
});

console.log('[EoH Live Advisor] Watching:', logFile);

if (!fs.existsSync(logFile)) {
    console.log('[EoH Live Advisor] Waiting for events.log to be created...');
}

let lastSize = 0;

async function analyzeEvent(line) {
    try {
        const completion = await client.chat.completions.create({
            model: 'gpt-5',
            messages: [
                {
                    role: 'system',
                    content: 'You are the Echoes of Humanity Live Advisor. Analyze DayZ server events and provide short operational insight.'
                },
                {
                    role: 'user',
                    content: line
                }
            ],
            max_tokens: 120
        });

        return completion.choices[0].message.content;
    }
    catch (err) {
        console.error('[EoH Live Advisor] OpenAI error:', err.message);
        return null;
    }
}

async function sendDiscordMessage(content) {
    if (!webhook) {
        return;
    }

    try {
        await fetch(webhook, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({
                username: 'EoH Live Advisor',
                content
            })
        });
    }
    catch (err) {
        console.error('[EoH Live Advisor] Discord webhook error:', err.message);
    }
}

chokidar.watch(logFile, { persistent: true })
.on('change', () => {
    const stats = fs.statSync(logFile);

    if (stats.size < lastSize) {
        lastSize = 0;
    }

    const stream = fs.createReadStream(logFile, {
        start: lastSize,
        end: stats.size
    });

    let data = '';

    stream.on('data', chunk => {
        data += chunk.toString();
    });

    stream.on('end', async () => {
        lastSize = stats.size;

        const lines = data.split('\n').filter(Boolean);

        for (const line of lines) {
            console.log('[EoH Event]', line);

            const analysis = await analyzeEvent(line);

            if (analysis) {
                console.log('[EoH Advisor]', analysis);

                await sendDiscordMessage(
                    '## EoH Live Advisor\n' +
                    '**Event**\n' +
                    '```\n' + line + '\n```\n' +
                    '**Analysis**\n' + analysis
                );
            }
        }
    });
});
