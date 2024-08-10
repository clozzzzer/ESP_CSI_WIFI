<template>
  <div class="show-csi">
    <div class="title-container">
      <div class="title-text">基于WI-FI信号的人员检测平台</div>
    </div>
    <div class="menu-container">
      <el-menu ref="menuRef" :default-active="activeIndex" class="el-menu-demo" mode="horizontal" @select="handleSelect" text-color="#409EFF" :router="true">
        <el-menu-item :disabled="!isLogin" index="devicedetail">WS</el-menu-item>
        <el-menu-item :disabled="!isLogin" index="home">首页</el-menu-item>
        <el-menu-item :disabled="!isLogin" index="history">历史日志</el-menu-item>
        <el-menu-item :disabled="!isLogin" index="client" is-dot>关联客户端</el-menu-item>
        <el-menu-item :disabled="!isLogin" @click.native="dialogVisible = true" >联系我们</el-menu-item>
      </el-menu>
    </div>
    <el-dialog
      title="联系方式"
      :visible.sync="dialogVisible"
      width="70%"
      :before-close="handleClose"
      modal>
        <div class="contact-info">
          <span>email:1401121306@qq.com</span>
          <span>version:1.0</span>
          <span>team:xxxxx</span>
          <span>如果有任何问题和修改意见，请及时联系我们！</span>
        </div>
      <span slot="footer" class="dialog-footer">
        <el-button type="primary" @click="dialogVisible = false">确 认</el-button>
      </span>
    </el-dialog>
  </div>
</template>

<script>
import { mapGetters } from 'vuex';

export default {
  data() {
    return {
      activeIndex: 'home',
      dialogVisible: false,
    };
  },
  computed: {
    ...mapGetters(['isLogin'])
  },
  methods: {
    handleSelect(key) {
      this.activeIndex = key;
    },
    open() {
      this.$notify({
        title: '联系方式',
        message: 'email:1401121306@qq.com',
        duration: 0
      });
    },
    handleClose() {
      this.dialogVisible = false;
    },
    adjustMenuWidth() {
      const menuWidth = this.$refs.menuRef.$el.offsetWidth;
      const container = document.querySelector('.menu-container');
      container.style.width = `${menuWidth}px`;
    }
  },
  mounted() {
    this.adjustMenuWidth();
    window.addEventListener('resize', this.adjustMenuWidth);
  },
  beforeDestroy() {
    window.removeEventListener('resize', this.adjustMenuWidth);
  }
};
</script>

<style scoped>
.show-csi {
  display: flex;
  flex-direction: column;
}

.title-container {
  text-align: center;
  background-color: #f5f5f5;
}

.title-text {
  font-family: 'MyCustomFont', sans-serif;
  font-size: 30px; /* 根据需要调整字体大小 */
  color: #333; /* 根据需要调整文字颜色 */
  padding: 10px 0;
}

.menu-container {
  overflow-x: auto;
  -webkit-overflow-scrolling: touch; /* Smooth scrolling on iOS devices */
}

.menu-container::-webkit-scrollbar {
  height: 6px;
}

.menu-container::-webkit-scrollbar-track {
  box-shadow: inset 0 0 6px rgba(0, 0, 0, 0.1);
  border-radius: 10px;
}

.menu-container::-webkit-scrollbar-thumb {
  border-radius: 10px;
  box-shadow: inset 0 0 6px rgba(0, 0, 0, 0.5);
}

.contact-info span {
  display: block;
  margin-bottom: 8px; /* 可选，用于增加每行之间的间距 */
}

@font-face {
  font-family: 'MyCustomFont';
  src: url('优设标题黑.ttf') format('truetype');
  font-weight: normal;
  font-style: normal;
}

@media(max-width: 768px) {
  
  .menu-container {
    overflow-x: auto; /* 添加水平滚动条 */
    -webkit-overflow-scrolling: touch; /* 平滑滚动效果 */
    width: 100%;
  }
  .title-text {
  font-size: 15px; /* 根据需要调整字体大小 */
}
}
</style>