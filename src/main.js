import Vue from 'vue';
import ElementUI from 'element-ui';
import 'element-ui/lib/theme-chalk/index.css';
import App from './App.vue';
import * as echarts from "echarts";
import router from './router/index';
import store from './store/index';
import { EventBus } from './event-bus.js';


Vue.prototype.$echarts = echarts;
Vue.use(ElementUI);

new Vue({
  el: '#app',
  render: h => h(App),
  router,
  store,
  created() {
    setInterval(() => {
      EventBus.$emit('update-data');
    }, 100);  // 每1000毫秒发射一次 'update-data' 事件
  }
});
