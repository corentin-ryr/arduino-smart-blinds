<template>
	<v-container>
		<h1>Title</h1>
		<!-- <li v-for="(value, index) in buttonsProperties" :key="value.name">
			<v-card class="mx-auto" max-width="344" outlined>
				<v-card-title>{{ value.name }}</v-card-title>
				<v-switch v-model="buttonsProperties[index].state" flat></v-switch>

			</v-card>
		</li> -->
		<Button v-bind:btnProperties="btn" :index="index" v-for="(btn, index) in buttonsProperties" :key="btn.name" v-on:updateSwitch="sendSwitchUpdate"></Button>

		<v-btn depressed v-on:click="onSwitchFromServer('cuisine', true)"> Normal </v-btn>
	</v-container>
</template>

<script>
	import Button from "@/components/Button"
	export default {
		name: "BtnList",

		components: {
			Button,
		},

		data: () => ({
			buttonsProperties: [],
			ESP_ip: "",
			ws: null,
			connected: false,
			waitingForPong: 0,
			isOnline: false,
		}),

		mounted() {
			this.ESP_ip = this.getESPlocation()
			this.wsInit()
			this.handlePingPong()
		},

		methods: {
			getESPlocation: function () {
				// var loc = location.hostname
				var loc = ""
				if (loc == "") loc = prompt("Enter ESP ip address", "192.168.0.104")
				return loc
			},

			wsInit: function () {
				this.ws = new WebSocket("ws://" + this.ESP_ip + ":81")

				this.ws.addEventListener("open", () => {
					this.connected = true
				})

				this.ws.addEventListener("message", (event) => {
					if (typeof event.data == "string") {
						this.receiveMessage(event.data)
					} else {
						//anything else is considered as a pong
						this.receivePong()
					}
				})
				this.ws.addEventListener("close", () => {
					this.connected = false
					setTimeout(this.wsInit, 1000)
				})
			},

			receiveMessage: function (message) {
				console.log("message")
				console.log(message)
				const commandIndex = parseInt(message.slice(0, 2))

				message = message.slice(2)
				message = message.replace(/'/g, '"')
				const obj = JSON.parse(message)

				switch (commandIndex) {
					case 1: //Answer to all states
						this.buttonsProperties = obj
						break

					case 2: //the server responds with "2 ok" in case of success of swith state updating
						break

					case 3: //Update on one of the buttons
						this.buttonsProperties[Object.keys(obj)[0]] = obj[Object.keys(obj)[0]].state
						break

					default:
						break
				}
			},

			receivePong: function () {
				this.waitingForPong = 0
				this.online()
			},

			handlePingPong: function () {
				if (this.connected) {
					if (this.waitingForPong++ > 2) {
						this.offline()
					}
					this.ws.send("")
				}
				setTimeout(this.handlePingPong, 1000)
			},

			online: function () {
				if (!this.isOnline) {
					this.isOnline = true
				}
			},

			offline: function () {
				if (this.isOnline) {
					this.isOnline = false
				}
			},

			onSwitchFromServer: function (btnName, switchSate) {
				//Trigered when a switch is updated physically and the server updates the client
				const btnIndex = this.buttonsProperties.findIndex((foo) => foo.name == btnName)
				this.buttonsProperties[btnIndex].state = switchSate
			},

			sendSwitchUpdate: function (btnIndex, switchSate) {
                this.buttonsProperties[btnIndex].state = switchSate
				this.ws.send("2 " + this.buttonsProperties[btnIndex].name + " " + switchSate)
			},
		},

		watch: {
			connected: function () {
				//On connected to the server, we ask for all the states with the names of the buttons
				if (this.connected == true) {
					this.ws.send("1") //1 means asking for all the states
				}
			},
		},
	}
</script>

<style scoped>
	li {
		list-style-type: none;
	}
</style>
