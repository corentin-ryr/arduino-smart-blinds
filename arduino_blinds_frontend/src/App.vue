<template>
  <v-app>
    <v-main>
      <BtnList/>
    </v-main>
  </v-app>
</template>

<script>
import BtnList from './components/BtnList';

export default {
  name: 'App',

  components: {
    BtnList,
  },

  data: () => ({
    //
  }),
};
</script>
