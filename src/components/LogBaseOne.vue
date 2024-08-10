<template>
  <div class="log-base-one" style="border-radius: 30px;display:flex">
    <div class="left">
        <i class="el-icon-time" style="margin-left:15px;margin-right:5px;font-size:25px"></i>
        历史日志
    </div>
    <div class="right" id="right">
        
    </div>
  </div>
</template>

<script>
export default {
    data () {
        return{
            input:'',
            currentTime: ''
        }
    }, 
    methods: {
      updateDateTime() {
      this.currentTime = new Date().toLocaleString();
    }
  },
    mounted() {
    this.updateDateTime(); // 初次调用以显示当前时间
    setInterval(this.updateDateTime, 1000); // 每秒更新一次时间
  },
    beforeDestroy() {
    if (this.interval) {
      clearInterval(this.interval); // 清除定时器以避免内存泄漏
    }
  }
}

</script>

<style>
.log-base-one {
  background-color: white;
  width: 70%;
  height: 7%;
  border: 1px solid white; /* 边框样式 */
  box-shadow: 0 2px 12px 0 rgba(0, 0, 0, 0.1);
  float: right;
  margin-top: 10px;
  display: flex;
  align-items: center; /* 垂直居中 */
}

.left, .right {
  width: 50%;
  height: 100%;
  display: flex;
  font-size: 20px;
}

.left {
  justify-content: flex-start; /* 默认就是从左开始排列 */
  align-items: center;
}

.right {
  justify-content: flex-end; /* 水平靠右 */
  align-items: center; /* 垂直居中 */
}

.right p {
  margin: 0;
}
</style>
