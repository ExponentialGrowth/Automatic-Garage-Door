const express = require('express');
const cors = require('cors');
const app = express();
const PORT = 5000;

app.use(cors());
app.use(express.json());

// In-memory state tracking
let currentDoorState = {
    status: "unknown",
    lastUpdated: new Date()
};

// Endpoint 1: Microcontroller posts live status updates here
app.post('/api/garage/telemetry', (req, res) => {
    const { status } = req.body;
    if (!status) {
        return res.status(400).json({ error: "Missing status value" });
    }
    
    currentDoorState = {
        status: status,
        lastUpdated: new Date()
    };
    
    console.log(`[Hardware Telemetry] Door is currently: ${status.toUpperCase()}`);
    return res.status(200).json({ success: true });
});

// Endpoint 2: Frontend Dashboard queries this to see if door is open or closed
app.get('/api/garage/status', (req, res) => {
    res.json(currentDoorState);
});

// Endpoint 3: UI Dashboard calls this to trigger the door trigger switch
app.post('/api/garage/toggle', (req, res) => {
    console.log("[Command Received] Triggering garage door action switch...");
    
    // In a production MQTT or WebSocket setup, you would broadcast 
    // an execution event to the physical microcontroller here.
    
    res.json({ 
        success: true, 
        message: "Trigger pulse command broadcasted successfully." 
    });
});

app.listen(PORT, () => {
    console.log(`Garage Door IoT Server running locally on port ${PORT}`);
});
