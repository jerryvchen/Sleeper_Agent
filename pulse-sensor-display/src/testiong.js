import React, { useState, useEffect } from "react";
import axios from "axios";

const PulseDataComponent = () => {
  const [pulseData, setPulseData] = useState([]);

  useEffect(() => {
    // Fetch pulse data from the backend
    const fetchPulseData = () => {
      axios
        .get("http://localhost:5000/pulse-data")
        .then((response) => {
          console.log(response);
          setPulseData(response.data); // Store the fetched data in state
        })
        .catch((error) => {
          console.error("Error fetching pulse data:", error);
        });
    };

    fetchPulseData();
    const interval = setInterval(fetchPulseData, 2000);
    return () => clearInterval(interval);
  }, []);

  return (
    <div>
      <h1>Pulse Data</h1>
      <table style={{ borderCollapse: "collapse", width: "100%" }}>
        <thead>
          <tr style={{ backgroundColor: "#f2f2f2" }}>
            <th style={{ border: "1px solid #ddd", padding: "8px" }}>Signal</th>
            <th style={{ border: "1px solid #ddd", padding: "8px" }}>
              Timestamp
            </th>
          </tr>
        </thead>
        <tbody>
          {pulseData.map((data, index) => (
            <tr
              key={index}
              style={{ textAlign: "center", borderBottom: "1px solid #ddd" }}
            >
              <td style={{ padding: "8px" }}>{data.signal}</td>
              <td style={{ padding: "8px" }}>
                {new Date(data.timestamp).toLocaleString()}
              </td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
};

export default PulseDataComponent;
