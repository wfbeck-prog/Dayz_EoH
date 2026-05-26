require('dotenv').config();

const fs = require('fs');
const chokidar = require('chokidar');

const logFile = process.env.EOH_EVENTS_FILE || './events.log';

console.log('[EoH Live Advisor] Watching:', logFile);

if (!fs.existsSync(logFile)) {
    console.log('[EoH Live Advisor] Waiting for events.log to be created...');
}

let lastSize = 0;

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

    stream.on('end', () => {
        lastSize = stats.size;

        const lines = data.split('\n').filter(Boolean);

        for (const line of lines) {
            console.log('[EoH Event]', line);
        }
    });
});
