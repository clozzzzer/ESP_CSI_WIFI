const { defineConfig } = require('@vue/cli-service');

module.exports = defineConfig({
  transpileDependencies: true,

  // 设置 publicPath 为相对路径
  publicPath: './',
  
  outputDir: 'dist',
  assetsDir: 'static'
});

