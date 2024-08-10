<template>
  <div class="base-one" style="border-radius: 30px;display:flex">
    <div class="avatar-wrapper">
      <el-avatar :size="70" src="https://cube.elemecdn.com/0/88/03b0d39583f48206768a7534e55bcpng.png">  </el-avatar>
    </div>
    <div class="right-content">
        <div class="row left-move">设备名</div>
        <div class="row right-move">位置</div>
        <div class="row right-move">状态</div>
        <div class="row left-move">历史警告</div>
    </div>
        <div class="right-content">
        <div class="row left-move" style="border:white"><el-button type="primary" size="medium"  style="min-width: 100px;min-height: 32px;" round>WS</el-button></div>
        <div class="row right-move" style="border:white"><el-button type="primary" size="medium" style="min-width: 100px;min-height: 32px;" round>卧室</el-button></div>
        <div v-if="isOnline" @click="isOnline=!isOnline" class="row right-move" style="border:white"><el-button type="success" size="medium" style="min-width: 100px;min-height: 32px;" round icon="el-icon-check">在线</el-button></div>
        <div v-else class="row right-move" @click="isOnline=!isOnline" style="border:white"><el-button type="danger" size="medium" style="min-width: 100px;min-height: 32px;" round icon="el-icon-close">离线</el-button></div>
        <div class="row left-move" style="border:white"><el-button type="warning" size="medium" style="min-width: 100px;min-height: 32px;" round>18</el-button></div>
    </div>
  </div>
</template>

<script>
export default {
    data () {
        return{
            isOnline:1
        }
    }
}
</script>

<style>
.base-one {
    background-color: white;
    width: 30%;
    height: 40%;
    border: 1px solid #ffffff; /* 边框样式 */
    box-shadow: 0 2px 12px 0 rgba(0, 0, 0, 0.1);
    float: right;
    margin-left: 30px;
    margin-top: 30px;
    display: flex;
    align-items: center; /* 垂直居中 */
}

.avatar-wrapper {
    margin-left: 10%; /* 水平离左边30% */
}

.right-content {
  display: flex;
  flex-direction: column;
  margin-left: 50px;
  align-items: center; /* 水平居中 */
  margin-top:20px;
  margin-left:20px;
}

.row {
  width: 100px; /* 定义容器的宽度 */
  height: 30px; /* 定义容器的高度 */
  border: 1px solid #409EFF; /* 添加边框 */
  margin-bottom: 20px; /* 设置容器之间的间距 */
  border-radius: 30px;
  display: flex; /* 添加 flex 布局 */
  align-items: center; /* 垂直居中 */
  justify-content: center; /* 水平居中 */
}

.left-move {
  margin-right: auto; /* 往左移动 */
}

.right-move {
  margin-left: auto; /* 往右移动 */
}
</style>