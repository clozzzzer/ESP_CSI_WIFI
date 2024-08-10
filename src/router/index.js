import DeviceDetail from '@/views/DeviceDetail'
import Home from '@/views/MyHome'
import History from '@/views/MyHistory'
import Client from '@/views/MyClient'
import Login from '@/views/MyLogin'
import Vue from 'vue'
import VueRouter from 'vue-router'
Vue.use(VueRouter) // VueRouter插件初始化

// 创建了一个路由对象
const router = new VueRouter({
  routes: [
    {path:'/', redirect:'/login'},
    { path: '/devicedetail', component: DeviceDetail },
    { path: '/home', component: Home },
    { path: '/history', component: History },
    { path: '/client', component: Client },
    { path: '/login', component:Login}
    //{ path: '/search/:words?', component: Search }
  ]
})

export default router