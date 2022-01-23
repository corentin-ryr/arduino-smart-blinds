<template>
	<div>
		<v-card class="mx-auto" max-width="344" outlined>
			<v-card-title>{{ btnProperties.name }}</v-card-title>
			<v-switch v-model="btnProperties.state" flat></v-switch>
		</v-card>
	</div>
</template>

<script>
	export default {
		name: "Button",
		props: {
			index: Number,
			btnProperties: Object,
		},
		data() {
			return {}
		},
		watch: {
			btnProperties: {
				handler(newValue) {
					console.log(newValue.state)
					this.$emit("updateSwitch", this.index, newValue.state)
				},
				deep: true,
			},
		},
	}
</script>
