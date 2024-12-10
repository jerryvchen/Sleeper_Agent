import React, { useState, useEffect } from "react";
import axios from "axios";
import { Line } from "react-chartjs-2";
import {
  Chart as ChartJS,
  CategoryScale,
  LinearScale,
  PointElement,
  LineElement,
  Title,
  Tooltip,
  Legend,
} from "chart.js";

// Register components
ChartJS.register(CategoryScale, LinearScale, PointElement, LineElement, Title, Tooltip, Legend);

const PulseDataComponent = () => {
  const [pulseData, setPulseData] = useState([]);
  const [chartData, setChartData] = useState({
    labels: [],
    datasets: [
      {
        label: "Pulse Signal",
        data: [],
        borderColor: "rgba(75, 192, 192, 1)",
        backgroundColor: "rgba(75, 192, 192, 0.2)",
        fill: true,
      },
    ],
  });

  useEffect(() => {
    const fetchPulseData = () => {
      axios
        .get("http://localhost:5000/pulse-data")
        .then((response) => {
          setPulseData(response.data);

          const labels = response.data.map((data) =>
            new Date(data.timestamp).toLocaleTimeString()
          );
          const dataPoints = response.data.map((data) => data.signal);

          setChartData({
            labels,
            datasets: [
              {
                label: "Pulse Signal",
                data: dataPoints,
                borderColor: "rgba(75, 192, 192, 1)",
                backgroundColor: "rgba(75, 192, 192, 0.2)",
                fill: true,
              },
            ],
          });
        })
        .catch((error) => {
          console.error("Error fetching pulse data:", error);
        });
    };

    fetchPulseData();
    const interval = setInterval(fetchPulseData, 200);
    return () => clearInterval(interval);
  }, []);

  return (
    <div>
      <h1>Pulse Data</h1>
      <div style={{ marginBottom: "20px" }}>
        <Line data={chartData} />
      </div>
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
              style={{
                textAlign: "center",
                borderBottom: "1px solid #ddd",
              }}
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
