<template>
    <el-main>
        <HomeBaseOne></HomeBaseOne>
    </el-main>
</template>

<script>
import HomeBaseOne from '@/components/HomeBaseOne.vue'

export default {
data() {
        return{
            text: ''
        }
    },
    components: {
        HomeBaseOne
    }
}
</script>

<style scoped>
.el-main {
        background-color: #ffffff;
        color: #333;
        display: flex;
        align-items: center;
        text-align: center;
        line-height: 50px;
        height: calc(100vh - 80px); /* 视口高度减去头部和尾部的高度 */
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