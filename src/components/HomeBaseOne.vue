<template>
  <div class="home-base-one" style="border-radius: 30px;display:flex">
    <div class="home-one" style="display:flex">
    <div>
      <el-avatar :size="70" fit="fill" style="margin-left:40px" :src="img3" alt="WS"></el-avatar>
    </div>
        <div class="right">
        <div class="row ">设备名</div>
        <div class="row ">状态</div>
    </div>
    <div class="right" >
        <div class="row " style="border:white"><el-button type="primary" size="small" style="min-width: 70px;min-height: 25px;"  round>WS</el-button></div>
        <div class="row " style="border:white"><el-button type="success" size="small" style="min-width: 70px;min-height: 25px;" round>Safe</el-button></div>
    </div>
    </div>
    <div class="home-two" style="display:flex">
    <div>
      <el-avatar :size="70" fit="fill" style="margin-left:40px" :src="img0"></el-avatar>
    </div>
   <div class="right">
        <div class="row ">设备名</div>
        <div class="row ">状态</div>
    </div>
    <div class="right">
        <div class="row " style="border:white"><el-button type="primary" size="small"  style="min-width: 70px;min-height: 25px;" round>KT</el-button></div>
        <div class="row " style="border:white"><el-button type="success" size="small" style="min-width: 70px;min-height: 25px;" round>Safe</el-button></div>
    </div>
        
    </div>
    <div class="home-three" style="display:flex">
        <div>
      <el-avatar :size="70" fit="fill" style="margin-left:40px" :src="img1"></el-avatar>
    </div>
            <div class="right">
        <div class="row ">设备名</div>
        <div class="row ">状态</div>
    </div>
        <div class="right">
        <div class="row " style="border:white"><el-button type="primary" size="small"  style="min-width: 70px;min-height: 25px;" round>KF</el-button></div>
        <div class="row " style="border:white"><el-button type="warning" size="small" style="min-width: 70px;min-height: 25px;" round>Warn</el-button></div>
    </div>
    </div>
    <div class="home-four" style="display:flex">
        <div>
      <el-avatar :size="70" fit="fill" style="margin-left:40px" :src="img2"></el-avatar>
    </div>
            <div class="right">
        <div class="row ">设备名</div>
        <div class="row ">状态</div>
    </div>
        <div class="right">
        <div class="row " style="border:white"><el-button type="primary" size="small"  style="min-width: 70px;min-height: 25px;" round>CT</el-button></div>
        <div class="row " style="border:white"><el-button type="warning" size="small" style="min-width: 70px;min-height: 25px;" round>Warn</el-button></div>
    </div>
    </div>
    
  </div>
</template>

<script>
import img2 from './pic/new3.webp'
import img1 from './pic/new2.png'
import img0 from './pic/new1.webp'
import img3 from './pic/new4.webp'
export default {
    data () {
        return{
            type0:true,
            type1:true,
            type2:true,
            type3:true,
            img2:img2,
            img1:img1,
            img0:img0,
            img3:img3
        }
    },
    methods:{

    }
}
</script>

<style>
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
.home-base-one {
    background-color: white;
    width: 100%;
    height: 100%;
    /*border: 1px solid #ccc; /* 边框样式 */
          
    float: right;
    display: flex;
    margin-top: 50px;
    flex-wrap: wrap;
    
}
.home-one {
    background-color: white;
    width: 30%;
    height: 40%;
border-radius: 30px;
background: #ffffff;
box-shadow:  -5px -5px 19px #999999,
             5px 5px 19px #ffffff;
    margin-left: 250px;
    display: flex;
    align-items: center; /* 垂直居中 */
}
.home-two {
    background-color: white;
    width: 30%;
    height: 40%;
border-radius: 30px;
background: #ffffff;
box-shadow:  5px -5px 19px #999999,
             -5px 5px 19px #ffffff;
    margin-left: 100px;
    display: flex;
    align-items: center; /* 垂直居中 */
    
}
.home-three {
    background-color: white;
    width: 30%;
    height: 40%;
border-radius: 30px;
background: #ffffff;
box-shadow:  -5px 5px 19px #999999,
             5px -5px 19px #ffffff;
  
    margin-left: 250px;
    display: flex;
    align-items: center; /* 垂直居中 */
}
.home-four {
    background-color: white;
    width: 30%;
    height: 40%;
border-radius: 30px;
background: #ffffff;
box-shadow:  5px 5px 19px #999999,
             -5px -5px 19px #ffffff;
    
     margin-left: 100px;
    display: flex;
    align-items: center; /* 垂直居中 */
}
.right {
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
  margin-bottom: 30px; /* 设置容器之间的间距 */
  border-radius: 30px;
  display: flex; /* 添加 flex 布局 */
  align-items: center; /* 垂直居中 */
  justify-content: center; /* 水平居中 */
}
</style>