         var button1 = document.getElementById("Key1");
			var button2 = document.getElementById("Key2");
			var button3 = document.getElementById("Key3");
			var button4 = document.getElementById("Key4");
			var button5 = document.getElementById("Key5");
			function power_key() {
				var request = new XMLHttpRequest();
				request.open('GET','/Pwr',false);
				request.send();
			}
			function up_key() {
				var request = new XMLHttpRequest();
				request.open('GET','/up',false);
				request.send();
			}
			function mode_key() {
				var request = new XMLHttpRequest();
				request.open('GET','/mode',false);
				request.send();
			}
			
			function down_key() {
				var request = new XMLHttpRequest();
				request.open('GET','/dwn',false);
				request.send();
			}
			function heater_key() {
				var request = new XMLHttpRequest();
				request.open('GET','/heat',false);
				request.send();
			}
			
			button1.addEventListener('click', power_key);
			button2.addEventListener('click', up_key);
			button3.addEventListener('click', mode_key);
			button4.addEventListener('click', down_key);
			button5.addEventListener('click', heater_key);
			