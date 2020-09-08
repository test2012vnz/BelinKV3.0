
var x = new XMLHttpRequest();
var isConnected = 0;
var ws = new WebSocket("wss://ssoc.solarbk.vn/websocket/mosca");
var apiReader = new FileReader();

ws.onopen = function (e) {
    ws.send(new Uint8Array([0x10]));
    ws.send(new Uint8Array([0x1e]));
    ws.send(new Uint8Array([0x00, 0x04]));
    ws.send(str2ab("MQTT"));
    ws.send(new Uint8Array([0x04]));
    ws.send(new Uint8Array([0x02]));
    ws.send(new Uint8Array([0x00, 0x0a]));
    ws.send(new Uint8Array([0x00, 0x12]));
    ws.send(str2ab("client-mkqq4wh879a"));
    console.log("[open] Connection established");

};

ws.onmessage = function (evt) {
    let recvMsg = evt.data;

    if (!isConnected) {
        var fileReader = new FileReader();
        fileReader.onload = function () {
            var array = new Uint8Array(this.result);
            console.log("Array contains", array.byteLength, "bytes.");
            //console.log(array);
            if (!isConnected) {
                if (array.toString() == new Uint8Array([0x20, 0x02, 0x00, 0x00])) {
                    console.log("OK");
                    //seriab();
                    isConnected = 1;
                    // 01-C016-0919-1560    // sunspec sma vincom
                    ws.send(new Uint8Array([0x82]));
                    ws.send(new Uint8Array([0x16, 0x14, 0xbe, 0x00, 0x11, 0x30, 0x31, 0x2d, 0x43, 0x30, 0x31, 0x36, 0x2d, 0x30, 0x39, 0x31, 0x39, 0x2d, 0x31, 0x35, 0x36, 0x30, 0x00]));
                    //01-C012-17-0006 huawei
                    ws.send(new Uint8Array([0x82]));
                    ws.send(new Uint8Array([0x14, 0x35, 0xe7, 0x00, 0x0f, 0x30, 0x31, 0x2d, 0x43, 0x30, 0x31, 0x32, 0x2d, 0x31, 0x37, 0x2d, 0x30, 0x30, 0x30, 0x36, 0x00]));
                    //01-C016-0219-0302 soliqQ
                    ws.send(new Uint8Array([0x82]));
                    ws.send(new Uint8Array([0x16, 0xe9, 0x56, 0x00, 0x11, 0x30, 0x31, 0x2d, 0x43, 0x30, 0x31, 0x36, 0x2d, 0x30, 0x32, 0x31, 0x39, 0x2d, 0x30, 0x33, 0x30, 0x32, 0x00]));
                    //01-C016-0219-0297 irex ck1 sungrow
                    ws.send(new Uint8Array([0x82]));
                    ws.send(new Uint8Array([0x16, 0x8f, 0xcc, 0x00, 0x11, 0x30, 0x31, 0x2d, 0x43, 0x30, 0x31, 0x36, 0x2d, 0x30, 0x32, 0x31, 0x39, 0x2d, 0x30, 0x32, 0x39, 0x37, 0x00]));
                    //01-C016-0119-0005 bigK
                    ws.send(new Uint8Array([0x82]));
                    ws.send(new Uint8Array([0x16, 0xd9, 0xbf, 0x00, 0x11, 0x30, 0x31, 0x2d, 0x43, 0x30, 0x31, 0x36, 0x2d, 0x30, 0x31, 0x31, 0x39, 0x2d, 0x30, 0x30, 0x30, 0x35, 0x00]));
                }
            }
        };
        fileReader.readAsArrayBuffer(recvMsg);
    }
    if (recvMsg.size > 10) {
        var str = blob2str(recvMsg);
        console.log(str);
        // var xhr = new XMLHttpRequest();
        // xhr.open("POST", "https://webhook.site/d86ec4d6-1bcd-4730-b246-027013cc5e10", true);
        // xhr.setRequestHeader('Content-Type', 'application/json');
        // xhr.send("a");
        http_post(str, "https://webhook.site/d86ec4d6-1bcd-4730-b246-027013cc5e10");

    }
}

function blob2str(b) {
    var u;
    u = URL.createObjectURL(b);
    x.open('GET', u, false); // although sync, not fetching over internet
    x.send();
    URL.revokeObjectURL(u);
    return x.responseText.substring(x.responseText.indexOf("{"), x.responseText.length);
}

function str2ab(str) {
    var buf = new ArrayBuffer(str.length); // 
    var bufView = new Uint8Array(buf);
    for (var i = 0, strLen = str.length; i < strLen; i++) {
        bufView[i] = str.charCodeAt(i);
    }
    return buf;
}

var myVar = setInterval(myTimer, 2000);
function myTimer() {
    if (isConnected) {
        ws.send(new Uint8Array([0xc0, 0x00]));
    }
}
function http_post(mess, url) {
    var xhttp = new XMLHttpRequest();
    xhttp.onload = function () {
        if (this.readyState == 4 && this.status == 200) {

        } else {

        }
    }
    xhttp.ontimeout = function () {

    }
    xhttp.timeout = 10000;
    xhttp.open("POST", url, true);
    xhttp.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
    xhttp.send(mess);
}