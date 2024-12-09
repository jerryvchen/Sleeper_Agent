const express = require('express');
const bodyParser = require('body-parser');
const app = express();
const port = 5000;
const cors = require('cors')

// Middleware to parse JSON data from the request body
app.use(bodyParser.json());
app.use(cors())

// Endpoint to receive pulse sensor data
let pulseDataArray = [];

app.post('/pulse-data', (req, res) => {
  const pulseData = req.body;  // Get the pulse data from the request body
  console.log("Received pulse data:", pulseData);  // Log the data to the console

  pulseDataArray.push(pulseData)
  console.log(pulseDataArray)

  // Send a response back to the ESP32
  res.status(200).send('Pulse data received');
});

app.get('/pulse-data', (req, res) => {
  res.status(200).json(pulseDataArray);
})

// Start the server
app.listen(port, () => {
  console.log(`Server is running at http://localhost:${port}`);
});

