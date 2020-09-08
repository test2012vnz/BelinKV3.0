
var http = require('http');
var zlib = require('zlib');
var WebSocket = require('ws');
const socket = new WebSocket.Server({ port: 81});
var fs = require('fs');
var qs  = require('querystring');
var contentType = "text/html";

var WS_START = "pkg={\"FIRMWARE\":\"iSTAR-v20191706\",\"TIME\":34563,\"TEST_MODE\":0,\"LIVE_TIME\":11140,\"PCF8563\":34652,\"RESET_COUNT\":579,\"WIFI_STATE\":3,\"WIFI_SIGNAL\":100,\"MQTT_STATE\":1,\"ADC1\":-273.15,\"ADC2\":-273.15 ,\"ADC3\":-273.15 ,\"RL1_LIVE\":17450 ,\"RL2_LIVE\":25781 ,\"RL3_LIVE\":24828}";
var ISTARJON = "{\"OK\":0,\"Serial\":\"BK-01-C019-0000-9999\",\"Reset\":9,\"Lifetime\":59422,\"Time\":1568278780,\"RS45_Mode\":1,\"RS45_Slave_ID\":0,\"connection\":{\"wifi\":{\"en\":1,\"connected\":3,\"ssid\":\"SolarBK-RD-LAB\",\"rssi\":66},\"eth\":{\"en\":1,\"DHCP\":1,\"ip\":3232235525,\"subnet\":4294967040,\"gateway\":3232235521},\"gsm\":{\"en\":0}},\"device\":{\"list_device\":\"[]\"},\"system\":{\"Application\":1,\"Info\":{\"TT_MAX\":80,\"TBON_MAX\":80,\"RCAP\":0,\"TANK_VOL\":1,\"TBS\":60,\"Manual\":0,\"Relay_Enable\":0},\"CONV\":{\"TYPE\":0,\"TON\":10,\"TOFF\":5,\"TIMEON\":600,\"TIMEOFF\":600},\"RETURN\":{\"TYPE\":0,\"TON\":10,\"TOFF\":5,\"TIMEON\":600,\"TIMEOFF\":600},\"HEATER\":{\"TON\":10},\"TimeSchedule\":{\"Num\":3,\"Schedule\":[25200,7200,46800,7200,68400,7200,0,0,0,0]}}}";
var OTHERJSON = "{\"OK\":0,\"Serial\":\"01-C019-0000-9999\",\"connection\":{\"wifi\":{\"en\":1},\"eth\":{\"en\":0,\"DHCP\":0,\"ip\":0,\"subnet\":0,\"gateway\":0},\"gsm\":{\"en\":0}},\"device\":{\"list_device\":\"[]\"},\"system\":{\"Application\":1,\"Manual\":0,\"p_Manual\":3,\"OTHERSLOT\":[{\"NUM\":5,\"SLOT\":[{\"TYPE\":2,\"TIMESTAR\":3660,\"DURATION\":3660,\"TIMEON\":600,\"TIMEOFF\":600},{\"TYPE\":3,\"TIMESTAR\":3660,\"DURATION\":3660,\"TIMEON\":0,\"TIMEOFF\":0},{\"TYPE\":3,\"TIMESTAR\":3660,\"DURATION\":3660,\"TIMEON\":0,\"TIMEOFF\":0},{\"TYPE\":3,\"TIMESTAR\":3660,\"DURATION\":3660,\"TIMEON\":0,\"TIMEOFF\":0},{\"TYPE\":3,\"TIMESTAR\":3660,\"DURATION\":3660,\"TIMEON\":0,\"TIMEOFF\":0}]},{\"NUM\":5,\"SLOT\":[{\"TYPE\":3,\"TIMESTAR\":3660,\"DURATION\":0,\"TIMEON\":0,\"TIMEOFF\":0},{\"TYPE\":3,\"TIMESTAR\":3660,\"DURATION\":3660,\"TIMEON\":0,\"TIMEOFF\":0},{\"TYPE\":3,\"TIMESTAR\":3660,\"DURATION\":3660,\"TIMEON\":0,\"TIMEOFF\":0},{\"TYPE\":3,\"TIMESTAR\":3660,\"DURATION\":3660,\"TIMEON\":0,\"TIMEOFF\":0},{\"TYPE\":3,\"TIMESTAR\":3660,\"DURATION\":3660,\"TIMEON\":0,\"TIMEOFF\":0}]},{\"NUM\":5,\"SLOT\":[{\"TYPE\":3,\"TIMESTAR\":3660,\"DURATION\":3660,\"TIMEON\":0,\"TIMEOFF\":0},{\"TYPE\":3,\"TIMESTAR\":3660,\"DURATION\":3660,\"TIMEON\":0,\"TIMEOFF\":0},{\"TYPE\":3,\"TIMESTAR\":3660,\"DURATION\":3660,\"TIMEON\":0,\"TIMEOFF\":0},{\"TYPE\":3,\"TIMESTAR\":3660,\"DURATION\":3660,\"TIMEON\":0,\"TIMEOFF\":0},{\"TYPE\":3,\"TIMESTAR\":3660,\"DURATION\":3660,\"TIMEON\":0,\"TIMEOFF\":0}]}]}}";
var device = 1;

var handleRequest = (request, response) => {
	var path = request.url.substring(1, request.url.lenght);
    if(path.endsWith('.gz')){
    	contentType = 'gzip';
    }else{
    	contentType = 'text/html';
    }
		response.writeHead(200, {
	        'Content-Type': contentType
	    });
    // zlib.gzip(text, function (_, result) {  // The callback will give you the 
    //   res.end(result);                     // result, so just send it.
    // });

    console.log(path);
	if(fs.existsSync(path)){
		fs.readFile(path, null, function (error, data) {
			if (error) {
				response.writeHead(404);
				response.write('Whoops! File not found!');
			} else {
				if(contentType=='gzip'){
				    zlib.gzip(data, function (_, result) {  // The callback will give you the 
				      res.end(result);                     // result, so just send it.
				    });
				}else{
					response.write(data);
				}
			}
			response.end();
		});
	}
	else{
		if(path=="c"){
			if(request.method=="POST"){
				var body = ''
				request.on('data', function(data) {
				  body += data
				})
				request.on('end', function() {
					var mes= "";
					var obj = qs.parse(body);
					console.log(body);
					switch(Number(obj.m)){
						case 3:{
							mes = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><wifi><net><ssid>hoangphong</ssid><rssi>92</rssi></net><net><ssid>Yen Nhi</ssid><rssi>66</rssi></net><net><ssid>3 Vedette</ssid><rssi>66</rssi></net><net><ssid>Phong 22A</ssid><rssi>58</rssi></net><net><ssid>18dtna2</ssid><rssi>52</rssi></net><net><ssid>Khong ten</ssid><rssi>38</rssi></net><net><ssid>PHONG 13B</ssid><rssi>36</rssi></net><net><ssid>DUY ANH</ssid><rssi>30</rssi></net><net><ssid>Nhu Hao</ssid><rssi>30</rssi></net><net><ssid>Hai Co Tien</ssid><rssi>28</rssi></net><net><ssid>Please Real Name!!!</ssid><rssi>26</rssi></net><net><ssid>P21b</ssid><rssi>24</rssi></net><net><ssid>Cary</ssid><rssi>16</rssi></net><net><ssid>Tuyet Van</ssid><rssi>16</rssi></net><net><ssid>Viet Tuong</ssid><rssi>16</rssi></net><net><ssid>PNT</ssid><rssi>16</rssi></net><net><ssid>P40A </ssid><rssi>16</rssi></net><net><ssid>Gia Dinh Nho</ssid><rssi>16</rssi></net></wifi>";
						}break;
						case 100:{
							mes = "[[1,15,1,0,0],[1,14,1,0,0],[1,15,2,0,0],[2,8,2,1,\"3232235521\"],[1,2,2,0,0],[1,1,2,0,0],[3,3,\"000000000001\",0,0],[1,2,3,0,0],[1,1,3,0,0],[1,10,3,0,0]]";
						}break;
						case 99:{
							// mes = ISTARJON;
							mes = "{\"ret\":1,\"RS45_Mode\":1,\"RS45_Slave_ID\":1,\"connection\":{\"netType\":1,\"wifi\":{\"en\":1,\"connected\":1,\"ssid\":\"SolarBK-RD-LAB\",\"rssi\":60},\"eth\":{\"en\":1,\"mac\":\"AA:AA:AA:AA:AA\",\"DHCP\":0,\"ip\":3232236290,\"subnet\":4294967040,\"gateway\":3232236289},\"gsm\":{\"en\":0,\"ser\":\"Mobi\",\"rssi\":90}},\"device\":{\"list_device\":\"[[1,1,1,0,0]]\"},\"info\":{\"Serial\":\"01-C019-0000-9999\",\"Reset\":107,\"Lifetime\":141257,\"Runningtime\":63566,\"sdsize\":7535,\"sdusedsize\":2036,\"totaldevice\":1,\"lastssoc\":0,\"wifi_condu\":5734,\"wifi_disdu\":0,\"eth_condu\":63573,\"eth_disdu\":0,\"gsm_condu\":0,\"gsm_disdu\":0}}";
						}break
						case 14:{
						}break;
						case 17:{	// rs485 master
							console.log(obj);
						}break;
					}
					if(Number(obj.m)==2){
						response.writeHead(200, {'Content-Type': 'text/xml'});
					}else{
						response.writeHead(200, {'Content-Type': contentType});
					}
					response.end(mes);
				})
			}
		}else{
			// response.writeHead(200, {'Content-Type': 'text/plain'});
			response.end();
		}
	}
};
http.createServer(handleRequest).listen(8100);

function removeByteOrderMark(str){
    return str.replace(/^\ufeff/g,"")
}
socket.on('connection', function connection(ws) {
 	ws.on('message', function incoming(message) {
 		console.log(message);
		if(message=='open'){
			ws.send(WS_START);
		}else{
		 	//var obj = JSON.parse(message);
		 	// if(obj.TEST == "ADC"){
		 	// 	ws.send("TEST={\"ADC\":[1,2,3]}"); 
		 	// }
		}
  	});
	ws.on('open', function open() {
	  ws.send('something');
	});
});

