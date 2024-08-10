<template>
 <div class="base-two" id="subcarrierAmplitudeChart">

 </div>
</template>

<script>
import * as echarts from 'echarts';
//import { EventBus } from '../event-bus.js';
export default {
  data() {
    return {
        
        default: [
                    [-6, 8, 8, -3, 3, -1, -9, 3, 1, 0, 3, 4, 8, -6, -6, 8, 0, -9, -3, 6, -10, -3, 6, 0, -5, -9, 6, 1, 0, 1],
        [-6, 8, 2, -3, 3, -1, -9, 3, 1, 0, 3, 4, 8, -6, -6, 8, 0, -9, 8, -6, 0, 8, 5, -9, -7, 7, -8, -9, 8, -8],
        [-6, 8, 2, -3, 4, -1, -8, 3, 1, 0, 3, 4, 7, -6, -7, 7, 0, -9, -3, 6, -10, -2, 6, 0, -5, -9, 6, 1, -1, 2],
        [-6, 8, 2, -3, 3, -1, -9, 3, 1, 0, 3, 4, 8, -6, -6, 8, 0, -9, 8, -6, -2, 8, 5, -9, -7, 7, -8, -8, 8, -8],
        [-6, 8, 2, -4, 4, -1, -9, 4, 1, 0, 3, 4, 7, -7, -8, 7, 0, -8, -3, 5, -10, -2, 6, 0, -5, -9, 6, 0, -2, 1],
        [-6, 8, 2, -3, 3, -1, -9, 3, 1, 0, 3, 4, 8, -6, -6, 8, 0, -9, 9, -6, -2, 8, 5, -9, -8, 8, -8, -8, 8, -8],
        [-6, 7, 3, -4, 4, -1, -9, 4, 0, 0, 3, 4, 6, -7, -9, 7, 0, -8, -2, 5, -10, -2, 7, 0, -5, -8, 5, 0, -3, 1],
        [-6, 8, 2, -3, 3, -1, -9, 3, 1, 0, 3, 4, 8, -6, -6, 8, 0, -9, 9, -6, -3, 9, 5, -8, -7, 7, -8, -7, 8, -8],
        [-6, 8, 2, -3, 3, -1, -9, 3, 1, 0, 3, 4, 8, -6, -6, 8, 0, -9, 9, -6, -3, 9, 5, -8, -7, 7, -8, -7, 8, -8],
        [-6, 8, 2, -3, 3, -1, -9, 3, 1, 0, 3, 4, 8, -6, -6, 8, 0, -9, 9, -6, -3, 9, 5, -8, -7, 7, -8, -7, 8, -8],
        [-6, 8, 2, -3, 3, -1, -9, 3, 1, 0, 3, 4, 8, -6, -6, 8, 0, -9, 9, -6, -3, 9, 5, -8, -7, 7, -8, -7, 8, -8]
        ],
        csi_data:[
            [],[],[],[],[],[],[],[],[],[],[],[]
        ],
        myChart : null,
        option:[],
        raw_csi_data:[],
        limit:0,
        buffer:[],
        raw_ws_data:[],
        temp:[
            [],[],[],[],[],[],[],[],[],[],[],[]
        ],
        isFirst:true,
        count:0
    };
  },

  methods: {
    transpose(matrix) {
        return matrix[0].map((col, i) => matrix.map(row => row[i]));
    },
    processCSV(csvData) {
        // 将 CSV 内容按行分割成数组
        const lines = csvData.trim().split('\n');

        // 初始化一个数组来存储解析后的数据
        let parsedData = [];

        // 遍历每一行数据
        for (let i = 0; i < lines.length; i++) {
        const line = lines[i].trim();

        // 跳过标题行，每隔 13 行为一个标题行
        if (i % 13 === 0) {
            continue;
        }

        // 解析数据行
        const cols = line.split(',');
        const rowData = cols.map(col => parseFloat(col.trim())); // 转为浮点数或适当的数据类型
        parsedData.push(rowData);
        }

        // 输出解析后的数据
        //console.log(parsedData);

        // 初始化一个新列表来存储重新组织后的数据
        let newDataList = [];

        // 定义每个子列表包含的行数
        const rowsPerList = 6;

        // 遍历 parsedData，并按照规则重新组织数据
        for (let i = 0; i < parsedData.length; i += rowsPerList) {
        let sublist = [];
        
        // 提取每组数据的第一个值放入前半部分
        for (let j = i; j < i + rowsPerList; j++) {
            sublist.push(parsedData[j][0]);
        }
        
        // 提取每组数据的第二个值放入后半部分
        for (let j = i; j < i + rowsPerList; j++) {
            sublist.push(parsedData[j][1]);
        }
        
        // 将当前子列表添加到 newDataList
        newDataList.push(sublist);
        }

        // 输出重新组织后的 newDataList
        //console.log(newDataList);
        this.raw_csi_data = this.transpose(newDataList);
        //console.log(this.csi_data);
    },
    async loadData() {
    try {
      const response = await fetch('/complex_data.csv'); 
      const csvData = await response.text();
      this.processCSV(csvData);
    } catch (error) {
      console.error('Error fetching the CSV file:', error);
    }
    },
    fetchData () {
        fetch('http://172.20.10.2:3000/api/datacsi')
            .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok');
            }
            return response.json();
            })
            .then(data => {
            this.buffer.push(data); // 
            
            })
            .catch(error => {
            console.error('Error fetching realtime data:', error);
            });
    },
    getWSData () {
      const ws = new WebSocket('ws://121.41.117.147:3000');

      ws.onopen = () => {
        console.log('WebSocket connection established');
      };

      ws.onmessage = (event) => {
        const data = JSON.parse(event.data);
        if(!this.isFirst)
        {
            this.loadWS(data);
            //console.log(data);
        }
        this.isFirst = false;
      };

      ws.onclose = () => {
        console.log('WebSocket connection closed');
      };

      ws.onerror = (error) => {
        console.error('WebSocket error:', error);
      };
    },
    loadWS (data) {
        //this.raw_ws_data.push(data);
        // console.log(data);
        //console.log(this.raw_ws_data);
        if(this.limit >= 50){
        this.csi_data = this.csi_data.map(sublist => {
        sublist.shift();
        return sublist;
        });
      }
        for(let index = 0;index<this.csi_data.length;index++)
        {
            if(index < 6)
            {
                //Real
                this.csi_data[index].push(data[index].Real);
            }
            else
            {
                //Imaginary
                this.csi_data[index].push(data[index - 6].Imaginary);
            } 
            
        }
        this.limit++;
        this.count++;
        //console.log(this.count)
        if(this.count === 20)
        {
            this.updateChart();
            this.count = 0
        }
        
    },
    updateWSData () {


        
        //this.csi_data = this.temp;
        //console.log(this.temp);
        //this.raw_ws_data = [];
/*         this.temp = [
            [],[],[],[],[],[],[],[],[],[],[],[]
        ] */
        //this.updateChart();
    },
    initChart() {
        var chartDom  = document.getElementById('subcarrierAmplitudeChart');
        this.myChart = echarts.init(chartDom);
        var i;

        

        var Xdata=[];

        for(i=0;i<50;i++)
        {
            Xdata.push(i+1);
        }
        
        this.option = {
        
        animationEasing: "linear",
        animationEasingUpdate: "quadraticIn",  //数据更新时的缓动效果
        animationDurationUpdate: 700,  //数据更新动画的时长
        animation: true ,//开启动画

        tooltip: {
            trigger: 'axis'
        },
        legend: {
            data: [
                {name:'1',icon:'roundRect'},{name:'2',icon:'roundRect'},{name:'3',icon:'roundRect'},{name:'4',icon:'roundRect'},
                {name:'5',icon:'roundRect'},{name:'6',icon:'roundRect'},{name:'7',icon:'roundRect'},{name:'8',icon:'roundRect'},
                {name:'9',icon:'roundRect'},{name:'10',icon:'roundRect'},{name:'11',icon:'roundRect'},{name:'12',icon:'roundRect'}
            ],
            y:20
        },
        grid: {
            left: '3%',
            right: '4%',
            bottom: '3%',
            containLabel: true
        },
        xAxis: {
            type: 'category',
            boundaryGap: false,
            data: Xdata
        },
        yAxis: {
            type: 'value'
        },
        series: [
            {
            name: '1',
            type: 'line',
            showSymbol:false,
            
            itemStyle: {
                        color: '#e30039',
                        shadowColor: '#e30039',
                        shadowBlur: 20,
                    },
                    lineStyle: {
                        shadowColor: "#e30039",
                        shadowOffsetX: 0,
                        shadowOffsetY: 0,
                        opacity: 1,
                        shadowBlur: 8,
                        type: "solid",
                        width:1,
                    },
            data: this.csi_data[0],
            },
            {
            name: '2',
            type: 'line',
            showSymbol:false,
            
            itemStyle: {
                        color: '#e30039',
                        shadowColor: '#e30039',
                        shadowBlur: 20,
                    },
                    lineStyle: {
                        shadowColor: "#e30039",
                        shadowOffsetX: 0,
                        shadowOffsetY: 0,
                        opacity: 1,
                        shadowBlur: 8,
                        type: "solid",
                        width:1,
                    },
            data: this.csi_data[1]
            },
            {
            name: '3',
            type: 'line',
            showSymbol:false,
            
            itemStyle: {
                        color: '#e30039',
                        shadowColor: '#e30039',
                        shadowBlur: 20,
                    },
                    lineStyle: {
                        shadowColor: "#e30039",
                        shadowOffsetX: 0,
                        shadowOffsetY: 0,
                        opacity: 1,
                        shadowBlur: 8,
                        type: "solid",
                        width:1,
                    },
            data: this.csi_data[2]
            },
            {
            name: '4',
            type: 'line',
            showSymbol:false,
            
            itemStyle: {
                        color: '#e30039',
                        shadowColor: '#e30039',
                        shadowBlur: 20,
                    },
                    lineStyle: {
                        shadowColor: "#e30039",
                        shadowOffsetX: 0,
                        shadowOffsetY: 0,
                        opacity: 1,
                        shadowBlur: 8,
                        type: "solid",
                        width:1,
                    },
            data: this.csi_data[3]
            },
            {
            name: '5',
            type: 'line',
            showSymbol:false,
            
            itemStyle: {
                        color: '#e30039',
                        shadowColor: '#e30039',
                        shadowBlur: 20,
                    },
                    lineStyle: {
                        shadowColor: "#e30039",
                        shadowOffsetX: 0,
                        shadowOffsetY: 0,
                        opacity: 1,
                        shadowBlur: 8,
                        type: "solid",
                        width:1,
                    },
            data: this.csi_data[4]
            },
            {
            name: '6',
            type: 'line',
            showSymbol:false,
            
            itemStyle: {
                        color: '#e30039',
                        shadowColor: '#e30039',
                        shadowBlur: 20,
                    },
                    lineStyle: {
                        shadowColor: "#e30039",
                        shadowOffsetX: 0,
                        shadowOffsetY: 0,
                        opacity: 1,
                        shadowBlur: 8,
                        type: "solid",
                        width:1,
                    },
            data: this.csi_data[5]
            },
            {
            name: '7',
            type: 'line',
            showSymbol:false,
            
            itemStyle: {
                        color: '#22ed7c',
                        shadowColor: '#22ed7c',
                        shadowBlur: 20,
                    },
                    lineStyle: {
                        shadowColor: "#22ed7c",
                        shadowOffsetX: 0,
                        shadowOffsetY: 0,
                        opacity: 1,
                        shadowBlur: 8,
                        type: "solid",
                        width:1,
                    },
            data: this.csi_data[6]
            },
            {
            name: '8',
            type: 'line',
            showSymbol:false,
            
            itemStyle: {
                        color: '#22ed7c',
                        shadowColor: '#22ed7c',
                        shadowBlur: 20,
                    },
                    lineStyle: {
                        shadowColor: "#22ed7c",
                        shadowOffsetX: 0,
                        shadowOffsetY: 0,
                        opacity: 1,
                        shadowBlur: 8,
                        type: "solid",
                        width:1,
                    },
            data: this.csi_data[7]
            },
            {
            name: '9',
            type: 'line',
            showSymbol:false,
            
            itemStyle: {
                        color: '#22ed7c',
                        shadowColor: '#22ed7c',
                        shadowBlur: 20,
                    },
                    lineStyle: {
                        shadowColor: "#22ed7c",
                        shadowOffsetX: 0,
                        shadowOffsetY: 0,
                        opacity: 1,
                        shadowBlur: 8,
                        type: "solid",
                        width:1,
                    },
            data: this.csi_data[8]
            },
            {
            name: '10',
            type: 'line',
            showSymbol:false,
            
            itemStyle: {
                        color: '#22ed7c',
                        shadowColor: '#22ed7c',
                        shadowBlur: 20,
                    },
                    lineStyle: {
                        shadowColor: "#22ed7c",
                        shadowOffsetX: 0,
                        shadowOffsetY: 0,
                        opacity: 1,
                        shadowBlur: 8,
                        type: "solid",
                        width:1,
                    },
            data: this.csi_data[9]
            },
            {
            name: '11',
            type: 'line',
            showSymbol:false,
            
            itemStyle: {
                        color: '#22ed7c',
                        shadowColor: '#22ed7c',
                        shadowBlur: 20,
                    },
                    lineStyle: {
                        shadowColor: "#22ed7c",
                        shadowOffsetX: 0,
                        shadowOffsetY: 0,
                        opacity: 1,
                        shadowBlur: 8,
                        type: "solid",
                        width:1,
                    },
            data: this.csi_data[10]
            },
            {
            name: '12',
            type: 'line',
            showSymbol:false,
            
            itemStyle: {
                        color: '#22ed7c',
                        shadowColor: '#22ed7c',
                        shadowBlur: 20,
                    },
                    lineStyle: {
                        shadowColor: "#22ed7c",
                        shadowOffsetX: 0,
                        shadowOffsetY: 0,
                        opacity: 1,
                        shadowBlur: 8,
                        type: "solid",
                        width:1,
                    },
            data: this.csi_data[11]
            },
            
        ],
            
        };

        this.option && this.myChart.setOption(this.option);
    },
    updateData() {
      // 对 csi_data 进行更新
      if(this.limit >= 15){
        this.csi_data = this.csi_data.map(sublist => {
        sublist.shift();
        return sublist;
        });
      }

        if(!this.buffer)
        {
            return;
        }
      // 添加新数据
       for(let index = 0;index<this.csi_data.length;index++)
       {
            if(index < 6)
            {
                //Real
                this.csi_data[index].push(this.buffer[this.limit][index].Real);
            }
            else
            {
                //Imaginary
                this.csi_data[index].push(this.buffer[this.limit][index-6].Imaginary);
            }
            
       }
       this.limit++;
       //this.currentindex++;
      // 重新渲染图表
      this.updateChart();
    },
    updateChart() {
      if (this.myChart) {
        this.myChart.setOption({
          series: this.csi_data.map((data, index) => ({
            name: String(index + 1),
            data
          }))
        });
      }
    },
  },
  created () {    

    this.getWSData();
  },
   mounted() {
    
    this.initChart();

    

    // 定义一个组件定时器，每0.5s向服务器发请求获得数据并存入缓冲区
/*     this.timer = setInterval(() => {
      this.updateChart();
    }, 500); */

    // 初始化定时器，每隔一秒更新数据和刷新图表
/*     EventBus.$on('update-data', this.updateChart); */
  },
/*   beforeDestroy() {
    // 组件销毁前停止监听，防止内存泄漏
    EventBus.$off('update-data', this.updateChart);
  } */
}
</script>

<style>
.base-two {
    background-color:white;
    width:60%;
    height:50%;
    border: 1px solid #ffffff; /* 边框样式 */
    box-shadow: 0 2px 12px 0 rgba(0, 0, 0, 0.1);
    float:right;
    margin-left:50px;
    margin-top:30px;
    border-radius: 30px;
}
</style>