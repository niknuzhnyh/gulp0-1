let qssid = 1111; // ! временные пароль и ссид вместо них будет переменная в которую будет записан ответ сервера
let qpass = 0000;

//! /////////////////////////////////////////////////////////

// получаем блоки из html

let authorization = document.querySelector(".authorization"); // блок авторизации
let success = document.querySelector(".success"); // сообщение об успешном подключении
let renouncement = document.querySelector(".renouncement"); // сообщение об ошибке

document.getElementById("btn-ok").addEventListener("click", () => {

   // получаем данные из формы 
   let ssid = document.getElementById("inpt-ssid").value;
   let pass = document.getElementById("inpt-pass").value;

   // отправляем
   let request = new XMLHttpRequest();
   request.open('GET', `/set?ssid=${ssid}&pass=${pass}`, true);
   request.send();
   
   // присваеваем класс .active необходимому блоку в зависимости от результата подключения
   if (ssid==qssid & pass==qpass) {
      authorization.classList.remove("active");
      success.classList.add("active");
   } else {
      authorization.classList.remove("active");
      renouncement.classList.add("active");
   }
});

// !функция показать пароль

function show_hide_password(target){
	let input = document.getElementById('inpt-pass');
	if (input.getAttribute('type') == 'password') {
		target.classList.add('view');
		input.setAttribute('type', 'text');
	} else {
		target.classList.remove('view');
		input.setAttribute('type', 'password');
	}
	return false;
}

// !функция показать пароль

document.getElementById("btn-sip-ok").addEventListener("click", () => {

   // получаем данные из формы 
   let ssid = document.getElementById("inpt-ssid").value;
   let pass = document.getElementById("inpt-pass").value;
   let ip = document.getElementById("ip").value;
   let gat = document.getElementById("gat").value;
   let mask = document.getElementById("mask").value;

   // !ip

   // разбиваем строку на массив 
   let arrayOfStrIp = ip.split('.');

   // делаем все элементы массива числами
   let arrayOfIntIp = arrayOfStrIp.map(Number)

   // записываем элементы массива в переменные
   let [ip1, ip2, ip3, ip4] = arrayOfIntIp;

   // !ip

   // !gat

   // разбиваем строку на массив 
   let arrayOfStrGat = gat.split('.');

   // делаем все элементы массива числами
   let arrayOfIntGat = arrayOfStrGat.map(Number)

   // записываем элементы массива в переменные
   let [gat1, gat2, gat3, gat4] = arrayOfIntGat;

   // !gat

   // !mask

   // разбиваем строку на массив 
   let arrayOfStrMask = mask.split('.');

   // делаем все элементы массива числами
   let arrayOfIntMask = arrayOfStrMask.map(Number)

   // записываем элементы массива в переменные
   let [mask1, mask2, mask3, mask4] = arrayOfIntMask;

   // !mask


   // отправляем
   let request = new XMLHttpRequest();
   request.open('GET', `/ipset?ssid=${ssid}&pass=${pass}ip1=${ip1}&ip2=${ip2}&ip3=${ip3}&ip4=${ip4}&gat1=${gat1}&gat2=${gat2}&gat3=${gat3}&gat4=${gat4}&mask1=${mask1}&mask2=${mask2}&mask3=${mask3}&mask4=${mask4}&`, true);
   request.send();
});