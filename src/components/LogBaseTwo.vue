<template>
  <div class="log-base-two" style="border-radius: 30px;display:flex">
      <el-table
    ref="filterTable"
    :data="tableData"
    style="width: 80%;border-radius: 30px;margin-left:20px;"
    height="100%">
    <el-table-column
      prop="date"
      label="日期"
      sortable
      width="150"
      column-key="date"
      :filters="dateList"
      :filter-method="filterHandler"
    >
    </el-table-column>
    <el-table-column
      prop="start"
      label="开始时间"
      width="150">
    </el-table-column>
        <el-table-column
      prop="end"
      label="结束时间"
      width="150">
    </el-table-column>
    <el-table-column
      prop="name"
      label="设备"
      width="150"
      :filters="nameList"
      :filter-method="filtername">
    </el-table-column>
    <el-table-column
      prop="location"
      label="位置"
      width="140"
    >
    </el-table-column>
    <el-table-column
      prop="tag"
      label="标签"
      width="140"
      :filters="tagList"
      :filter-method="filterTag"
      filter-placement="bottom-end">
      <template slot-scope="scope">
        <el-tag
          v-if="scope.row.tag==='alert'" type="danger" effect="dark"
          disable-transitions>{{scope.row.tag}}</el-tag>
        <el-tag
          v-else-if="scope.row.tag==='safe'" type="success" effect="dark"
          disable-transitions>{{scope.row.tag}}</el-tag>
      </template>
    </el-table-column>
    <el-table-column
      label="操作"
      >
      <template slot-scope="scope">
        <el-button @click="handleClick(scope.row.id)" type="text" size="small">已读</el-button>
      </template>
    </el-table-column>
  </el-table>
  </div>
</template>

<script>
export default {
    data() {
        return {
            tagList:[{ text: 'alert', value: '1' }, { text: 'safe', value: '0' }],
            dateList:[{text: '2024-05-28', value: '2024-05-28'}, {text: '2024-05-29', value: '2024-05-29'}, {text: '2024-05-30', value: '2024-05-30'}],
            nameList:[{text:'WS',value:'WS'}],
            tableData: [],
            isFirst : true
        }
    },
    created() {
      this.getWSData();
    },
        methods: {
      filterTag(value, row) {
        return row.tag === value;
      },
      filterHandler(value, row) {
        return row.date === value;
      },
      filtername(value,row) {
        return row.name === value;
      },
      getWSData () {
      const ws = new WebSocket('ws://127.0.0.1:5000');

      ws.onopen = () => {
        console.log('WebSocket connection established');
      };

      ws.onmessage = (event) => {
        const data = JSON.parse(event.data);
        if(!this.isFirst)
        {
            this.loadWS(data);
            //console.log(data);
        }
        this.isFirst = false;
      };

      ws.onclose = () => {
        console.log('WebSocket connection closed');
      };

      ws.onerror = (error) => {
        console.error('WebSocket error:', error);
      };
      },
      loadWS (data) {
        //btime stime type
/*         let date = 0;
        let start = 0;
        let stop = 0; */
        let tag = null;
        console.log(data)
        for(let i = data.length-1;i>=0;i--)
        {
          if(data[i].type1 === '0')
          { tag = 'safe'}
          else
          {
             tag = 'alert'
          }
          this.tableData.push({
            date: data[i].sdate,
            start: data[i].btime,
            end: data[i].stime,
            name: 'WS',
            location:'卧室',
            tag: tag,
            id : data.length - i + 1
          })
        }
      },
      handleClick (id) {
        this.tableData = this.tableData.filter(item => item.id !== id);
      }
    }
}
</script>

<style>
.log-base-two {
    background-color: white;
    width: 90%;
    height: 80%;
    border: 1px solid #ffffff; /* 边框样式 */
    float: right;
    margin-top: 50px;
    display: flex;
    box-shadow: 0 2px 12px 0 rgba(0, 0, 0, 0.1);
    align-items: center; /* 垂直居中 */
}
</style>