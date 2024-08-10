<template>
  <div class="base-four" id="main">
    
  </div>
</template>


<script>
import * as echarts from "echarts";
// import axios from 'axios'; // 如果你选择使用 Axios
// import { EventBus } from '../event-bus.js';
export default {
  data() {
    return {
      myChart: null,
      option: null,
      timer: null,
      countlist: [],
      Movelist:[],
      limit:0,
      index:0,
      buffer:[],
      isFirst:true,
    };
  },
  methods: {
    initChart() {
      // 初始化图表
      var chartDom = document.getElementById('main');
      this.myChart = echarts.init(chartDom);
      var timelist = [];
      this.countlist = []; // 初始化 countlist 数组

      // 生成初始数据
      for (var i = 1; i <= 30; i++) {
        timelist.push(i);
        // this.countlist.push(0); // 初始值为 0
      }

      // 初始化图表配置
      this.option = {
        animationDurationUpdate: 700,  //数据更新动画的时长
        animation: true ,//开启动画
        legend: {
          data: ['Someone','Move'],
          y: 10
        },
        xAxis: {
          type: 'category',
          data: timelist
        },
        yAxis: {
          type: 'value'
        },
        grid: {
          y: 40,
          y2: 25,
          x: 60,
          x2: 30
        },
        series: [
          {
            name: 'Someone',
            data: this.countlist,
            type: 'bar',
            itemStyle: {
              normal: {
                color: "#409EFF",
                barBorderRadius: [7, 7, 0, 0],
                shadowColor: "#409EFF",
                shadowOffsetX: 0,
                shadowOffsetY: 0,
                shadowBlur: 8
              }
            }
          },
                    {
            name: 'Move',
            data: this.countlist,
            type: 'bar',
            itemStyle: {
              normal: {
                color: "#71ae46",
                barBorderRadius: [7, 7, 0, 0],
                shadowColor: "#71ae46",
                shadowOffsetX: 0,
                shadowOffsetY: 0,
                shadowBlur: 8
              }
            }
          },
        ]
      };

      // 设置图表配置
      this.option && this.myChart.setOption(this.option);
    },
    updateData() {
            if(this.limit>=15)
            {
                this.countlist.shift(); 
            }// 移除数组的第一个元素
            if(this.limit<15)
            {
                this.limit++;
            }
            this.countlist.push(this.buffer[this.index]); // 添加新的随机数值到数组末尾
            this.index++;
            // 更新图表数据
            this.myChart.setOption({
                series: [
                {
                    name: 'Move',
                    data: this.countlist
                }
                ]
            });
    },
    fetchData () {
        fetch('http://localhost:3000/api/datamove')
            .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok');
            }
            return response.json();
            })
            .then(data => {
            this.buffer.push(data.Value); // 响应数据结构 { Value: 0|1 }
            })
            .catch(error => {
            console.error('Error fetching realtime data:', error);
            });
    },
    getWSData () {
      const ws = new WebSocket('ws://172.24.231.54:8000');

      ws.onopen = () => {
        console.log('WebSocket connection established');
      };

      ws.onmessage = (event) => {
        const data = JSON.parse(event.data);
        if(!this.isFirst)
        {
          this.loadWS(data);
          
        }
        this.isFirst = false
      };

      ws.onclose = () => {
        console.log('WebSocket connection closed');
      };

      ws.onerror = (error) => {
        console.error('WebSocket error:', error);
      };
    },
    loadWS (data) {
            if(this.limit>=30)
            {
                this.countlist.shift(); 
                this.Movelist.shift();
            }// 移除数组的第一个元素
            if(this.limit<30)
            {
                this.limit++;
            }
            this.countlist.push(data[0].someone); // 添加新的随机数值到数组末尾
            this.Movelist.push(data[0].move)
            // 更新图表数据
            this.myChart.setOption({
                series: [
                {
                    name: 'Someone',
                    data: this.countlist
                },
                {
                    name: 'Move',
                    data: this.Movelist
                }
                ]
            });
    }
  },
  created () {
    this.getWSData();
  },
  mounted() {
    // 初始化图表
    this.initChart();

/*     // 定义一个组件定时器，每0.5s向服务器发请求获得数据并存入缓冲区

    this.timer = setInterval(() => {
      this.fetchData();
    }, 500);

    // 初始化定时器，每隔一秒更新数据和刷新图表
    EventBus.$on('update-data', this.updateData); */
  },
  beforeDestroy() {
/*     // 组件销毁前停止监听，防止内存泄漏
    EventBus.$off('update-data', this.updateData);
    clearInterval(this.timer); */
  }
};
</script>

<style>
.base-four {
    background-color:white;
    width:60%;
    height:40%;
    border: 1px solid #ffffff; /* 边框样式 */
    box-shadow: 0 2px 12px 0 rgba(0, 0, 0, 0.1);
    float:right;
    margin-left: 50px;
    border-radius: 30px;
}
</style>