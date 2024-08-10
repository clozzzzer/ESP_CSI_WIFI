<template>
  <div class="App">
        <el-container>
          <ShowCSI></ShowCSI>
          <router-view></router-view>
        <el-footer></el-footer>
    </el-container>
  </div>
</template>


<script>
import ShowCSI from './components/ShowCSI.vue'

export default {
data() {
        return{
           
            text: ''
        }
    },
    components: {
         ShowCSI,
    }
}
</script>

<style>
        .el-header, .el-footer {
        background-color: white;
        color: #333;
        border: #333;
        line-height: 60px;
        padding-left: 20px; /* 添加内边距以确保文本不紧贴边缘 */
        }

        .el-header {
        text-align: left; /* 将header中的文本对齐到左侧 */
        }

        .el-footer {
        text-align: left; /* 保持footer文本居中 */
        }

        .el-main {
        background-color: #ffffff;
        color: #333;
        display: flex;
        justify-content: space-between; /* 左右对齐 */
        text-align: center;
        line-height: 50px;
        height: calc(100vh - 120px); /* 视口高度减去头部和尾部的高度 */
        width: 100%;
        flex-wrap: wrap;
         flex-direction: column; /* 将元素排列方向改为竖直 */
        }

        body > .el-container {
        display: flex;
        flex-direction: column;
        height: 100vh;
        margin-bottom: 0; /* 去掉底部外边距 */
        }

        .el-header, .el-footer {
        flex-shrink: 0; /* 头部和尾部固定高度 */
        }




    
    .el-container:nth-child(5) .el-aside,
    .el-container:nth-child(6) .el-aside {
      line-height: 260px;
    }
    
    .el-container:nth-child(7) .el-aside {
      line-height: 320px;
    }
</style>
