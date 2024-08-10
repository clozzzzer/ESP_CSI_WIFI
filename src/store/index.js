// src/store/index.js

import Vue from 'vue';
import Vuex from 'vuex';

Vue.use(Vuex);

// 定义插件
const localStoragePlugin = store => {
  store.subscribe((mutation, state) => {
    localStorage.setItem('store', JSON.stringify(state));
  });
};

// 尝试从localStorage中加载状态
const savedState = localStorage.getItem('store');

// 创建Vuex Store
export default new Vuex.Store({
  state: savedState ? JSON.parse(savedState) : {
    isLogin: false
  },
  mutations: {
    setLogin(state, newLogin) {
      state.isLogin = newLogin;
    }
  },
  actions: {
    updateLogin({ commit }, newLogin) {
      commit('setLogin', newLogin);
    }
  },
  getters: {
    isLogin: state => state.isLogin
  },
  plugins: [localStoragePlugin]
});

