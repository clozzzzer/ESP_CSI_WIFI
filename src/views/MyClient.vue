<template>
    <el-main>
        <ClientBaseOne></ClientBaseOne>
    </el-main>
</template>

<script>
import ClientBaseOne from '@/components/ClientBaseOne.vue'

export default {
data() {
        return{
            text: ''
        }
    },
    components: {
        ClientBaseOne
    }
}
</script>

<style scoped>
        .el-main {
        background-color: #ffffff;
        background-image: url("./cool-background.png");
        background-size: 100% 100%;
        background-repeat: no-repeat;
        color: #333;
        display: flex;
        justify-content: space-between; /* 左右对齐 */
        text-align: center;
        align-items: center;
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
</style>