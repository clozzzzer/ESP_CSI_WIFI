<template>
<div class="form-container">
<div class="card">
    <span class="card__title">关联客户端</span>
    <p class="card__content">关联移动设备以第一时间获取消息.
    </p>
    <div class="card__form">
        <input placeholder="Telephone" type="text">
        <button class="sign-up">关联</button>
    </div>
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
.client-base-one {
    background-color: white;
    width: 30%;
    height: 40%;
    border: 1px solid #ccc; /* 边框样式 */
    float: right;
    margin-left: 30px;
    margin-top: 30px;
    display: flex;
    align-items: center; /* 垂直居中 */
    
}
.card {
    margin-top:30%;
  width: 240px;
  height: 254px;
  padding: 0 15px;
  text-align: center;
  display: flex;
  align-items: center;
  justify-content: center;
  flex-direction: column;
  gap: 12px;
  background: #fff;
  border-radius: 20px;
    box-shadow: 0 2px 12px 0 rgba(0, 0, 0, 0.1);
}

.card > * {
  margin: 0;
}

.card__title {
  font-size: 23px;
  font-weight: 900;
  color: #333;
}

.card__content {
  font-size: 13px;
  line-height: 18px;
  color: #333;
}

.card__form {
  display: flex;
  flex-direction: column;
  gap: 10px;
}

.card__form input {
  margin-top: 10px;
  outline: 0;
  background: rgb(255, 255, 255);
  box-shadow: transparent 0px 0px 0px 1px inset;
  padding: 0.6em;
  border-radius: 14px;
  border: 1px solid #333;
  color: black;
}

.card__form button {
  border: 0;
  background: #409EFF;
  color: #fff;
  padding: 0.68em;
  border-radius: 14px;
  font-weight: bold;
}

.sign-up:hover {
  opacity: 0.8;
} 


</style>