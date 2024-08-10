// server.js

const express = require('express');
const fs = require('fs');
const path = require('path');
const csv = require('csv-parser');
const cors = require('cors');

const app = express();
const PORT = process.env.PORT || 3000;
app.use(cors());

// 存储当前已经发送的行数
let currentLine = 0;
const csvFilePath = path.join(__dirname, 'random_numbers.csv'); // CSV 文件的路径

// 读取 CSV 文件到数组中
const readCsvFile = () => {
  return new Promise((resolve, reject) => {
    const rows = [];
    fs.createReadStream(csvFilePath)
      .pipe(csv())
      .on('data', (row) => rows.push(row))
      .on('end', () => resolve(rows))
      .on('error', (error) => reject(error));
  });
};

// 获取特定行的数据
app.get('/api/data', async (req, res) => {
  try {
    const rows = await readCsvFile();
    if (currentLine < rows.length) {
      res.json(rows[currentLine]); // 返回当前行的数据
      currentLine++; // 增加行数
      if(currentLine == rows.length)
        {
            currentLine = 0;
        }
    } else {
      res.status(404).json({ message: 'No more data available' });
    }
  } catch (error) {
    res.status(500).json({ message: 'Error reading CSV file', error });
  }
});

app.listen(PORT, () => {
  console.log(`Server is running on http://localhost:${PORT}`);
});
