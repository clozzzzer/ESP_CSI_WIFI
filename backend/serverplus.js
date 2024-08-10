const express = require('express');
const fs = require('fs');
const path = require('path');
const csv = require('csv-parser');
const cors = require('cors');

const app = express();
const PORT = process.env.PORT || 3000;
app.use(cors());

// 存储当前已经发送的行数
let currentLineMove = 0;
let currentLineCsi = 0;
const moveCsvFilePath = path.join(__dirname, 'movedata.csv'); // Move 数据 CSV 文件的路径
const csiCsvFilePath = path.join(__dirname, 'csidata.csv'); // CSI 数据 CSV 文件的路径

// 读取 Move CSV 文件中的数据（适应单列数据）
const readMoveStream = async (startLine, endLine) => {
  return new Promise((resolve, reject) => {
    const rows = [];
    let lineCount = 0;

    fs.createReadStream(moveCsvFilePath)
      .pipe(csv())
      .on('data', (row) => {
        lineCount++;
        if (lineCount > startLine && lineCount <= endLine) {
          rows.push(row.Value); // 读取 `Value` 列
        }
      })
      .on('end', () => resolve(rows))
      .on('error', (error) => reject(error));
  });
};

// 读取 CSI CSV 文件中的数据
const readCsiStream = async (startLine, endLine) => {
    return new Promise((resolve, reject) => {
      const rows = [];
      let lineCount = 0;
  
      fs.createReadStream(csiCsvFilePath)
        .pipe(csv())
        .on('data', (row) => {
          lineCount++;
          let noTitle = true;
          // 跳过标题行
          if(row.Real == 'Real' && row.Imaginary == 'Imaginary')
          {
            noTitle = false;
            lineCount--;
          }
          if (lineCount > startLine && lineCount <= endLine && noTitle) {
            rows.push({
              Real: row.Real,
              Imaginary: row.Imaginary
            });
          }
        })
        .on('end', () => resolve(rows))
        .on('error', (error) => reject(error));
    });
  };

// 获取特定行的 Move 数据
app.get('/api/datamove', async (req, res) => {
  try {
    const dataChunkSize = 1; // 每次返回 1 行数据
    const endLine = currentLineMove + dataChunkSize;

    // 读取数据流中的特定行
    const dataChunk = await readMoveStream(currentLineMove, endLine);

    if (dataChunk.length > 0) {
      res.json({ Value: dataChunk[0] }); // 返回当前行的数据
      currentLineMove = endLine; // 更新当前行数
    } else {
       
      res.json({ Value: 0 });
    }
  } catch (error) {
    res.status(500).json({ message: 'Error reading Move file', error });
  }
});

// 获取特定行的 CSI 数据
app.get('/api/datacsi', async (req, res) => {
  try {
    const dataChunkSize = 6; // 每次返回 6 行数据
    const endLine = currentLineCsi + dataChunkSize;

    // 读取数据流中的特定行
    const dataChunk = await readCsiStream(currentLineCsi, endLine);

    if (dataChunk.length > 0) {
      res.json(dataChunk); // 返回当前数据块
      currentLineCsi = endLine; // 更新当前行数
      if (dataChunk.length < dataChunkSize) {
        currentLineCsi = 0; // 如果到达文件末尾，重置行数
      }
    } else {
      res.status(404).json({ message: 'No more CSI data available' });
    }
  } catch (error) {
    res.status(500).json({ message: 'Error reading CSI file', error });
  }
});

app.listen(PORT, () => {
  console.log(`Server is running on http://localhost:${PORT}`);
});
