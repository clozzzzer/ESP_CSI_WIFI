<template>
 <div class="base-three">
        <el-table
    :data="tableData"
    height="100%"
    style="width: 100%;border-radius: 30px;margin-left:15px">
    <el-table-column
      prop="Device"
      label="设备"
      width="110">
    </el-table-column>
    <el-table-column
      prop="location"
      label="位置"
      width="110">
    </el-table-column>
    <el-table-column
      prop="ishandled"
      label="异常待处理"
      width="110">
    <template slot-scope="scope">
           <el-button @click="scope.row.ishandled=!scope.row.ishandled" size="mini"  :type="scope.row.ishandled === true ? 'success' : 'danger'" :icon="scope.row.ishandled === true ? 'el-icon-check' : 'el-icon-close'" circle></el-button>
      </template>
    </el-table-column>
    <el-table-column
     
      label="操作"
      width="60">
      <template slot-scope="scope">
        <el-button @click="handleClick(scope.row)" type="text" size="small">查看</el-button>
      </template>
    </el-table-column>
  </el-table>
 </div>
</template>

<script>
export default {
    data() {
      return {
        tableData: [{
          Device: 'WS',
          location: '卧室',
          ishandled: true
        }, {
          Device: 'KT',
          location: '客厅',
          ishandled: true
        }, {
          Device: 'KF',
          location: '客房',
          ishandled: false
        }, {
          Device: 'CT',
          location: '餐厅',
          ishandled: true
        }]
      }
    }
  }
</script>

<style>
.base-three {
    background-color:white;
    width:30%;
    height:50%;
    border: 1px solid #ffffff; /* 边框样式 */
    box-shadow: 0 2px 12px 0 rgba(0, 0, 0, 0.1);
    float:right;
    margin-left: 30px;
    border-radius: 30px;
    line-height: 40px;
    display: flex;
    align-items: right;
}
</style>