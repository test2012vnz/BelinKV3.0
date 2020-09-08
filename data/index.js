(function (factory) { if (typeof exports === "object") { module.exports = factory() } else if (typeof define === "function" && define.amd) { define(factory) } else { var glob; try { glob = window } catch (e) { glob = self } glob.SparkMD5 = factory() } })(function (undefined) { "use strict"; var add32 = function (a, b) { return a + b & 4294967295 }, hex_chr = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f"]; function cmn(q, a, b, x, s, t) { a = add32(add32(a, q), add32(x, t)); return add32(a << s | a >>> 32 - s, b) } function md5cycle(x, k) { var a = x[0], b = x[1], c = x[2], d = x[3]; a += (b & c | ~b & d) + k[0] - 680876936 | 0; a = (a << 7 | a >>> 25) + b | 0; d += (a & b | ~a & c) + k[1] - 389564586 | 0; d = (d << 12 | d >>> 20) + a | 0; c += (d & a | ~d & b) + k[2] + 606105819 | 0; c = (c << 17 | c >>> 15) + d | 0; b += (c & d | ~c & a) + k[3] - 1044525330 | 0; b = (b << 22 | b >>> 10) + c | 0; a += (b & c | ~b & d) + k[4] - 176418897 | 0; a = (a << 7 | a >>> 25) + b | 0; d += (a & b | ~a & c) + k[5] + 1200080426 | 0; d = (d << 12 | d >>> 20) + a | 0; c += (d & a | ~d & b) + k[6] - 1473231341 | 0; c = (c << 17 | c >>> 15) + d | 0; b += (c & d | ~c & a) + k[7] - 45705983 | 0; b = (b << 22 | b >>> 10) + c | 0; a += (b & c | ~b & d) + k[8] + 1770035416 | 0; a = (a << 7 | a >>> 25) + b | 0; d += (a & b | ~a & c) + k[9] - 1958414417 | 0; d = (d << 12 | d >>> 20) + a | 0; c += (d & a | ~d & b) + k[10] - 42063 | 0; c = (c << 17 | c >>> 15) + d | 0; b += (c & d | ~c & a) + k[11] - 1990404162 | 0; b = (b << 22 | b >>> 10) + c | 0; a += (b & c | ~b & d) + k[12] + 1804603682 | 0; a = (a << 7 | a >>> 25) + b | 0; d += (a & b | ~a & c) + k[13] - 40341101 | 0; d = (d << 12 | d >>> 20) + a | 0; c += (d & a | ~d & b) + k[14] - 1502002290 | 0; c = (c << 17 | c >>> 15) + d | 0; b += (c & d | ~c & a) + k[15] + 1236535329 | 0; b = (b << 22 | b >>> 10) + c | 0; a += (b & d | c & ~d) + k[1] - 165796510 | 0; a = (a << 5 | a >>> 27) + b | 0; d += (a & c | b & ~c) + k[6] - 1069501632 | 0; d = (d << 9 | d >>> 23) + a | 0; c += (d & b | a & ~b) + k[11] + 643717713 | 0; c = (c << 14 | c >>> 18) + d | 0; b += (c & a | d & ~a) + k[0] - 373897302 | 0; b = (b << 20 | b >>> 12) + c | 0; a += (b & d | c & ~d) + k[5] - 701558691 | 0; a = (a << 5 | a >>> 27) + b | 0; d += (a & c | b & ~c) + k[10] + 38016083 | 0; d = (d << 9 | d >>> 23) + a | 0; c += (d & b | a & ~b) + k[15] - 660478335 | 0; c = (c << 14 | c >>> 18) + d | 0; b += (c & a | d & ~a) + k[4] - 405537848 | 0; b = (b << 20 | b >>> 12) + c | 0; a += (b & d | c & ~d) + k[9] + 568446438 | 0; a = (a << 5 | a >>> 27) + b | 0; d += (a & c | b & ~c) + k[14] - 1019803690 | 0; d = (d << 9 | d >>> 23) + a | 0; c += (d & b | a & ~b) + k[3] - 187363961 | 0; c = (c << 14 | c >>> 18) + d | 0; b += (c & a | d & ~a) + k[8] + 1163531501 | 0; b = (b << 20 | b >>> 12) + c | 0; a += (b & d | c & ~d) + k[13] - 1444681467 | 0; a = (a << 5 | a >>> 27) + b | 0; d += (a & c | b & ~c) + k[2] - 51403784 | 0; d = (d << 9 | d >>> 23) + a | 0; c += (d & b | a & ~b) + k[7] + 1735328473 | 0; c = (c << 14 | c >>> 18) + d | 0; b += (c & a | d & ~a) + k[12] - 1926607734 | 0; b = (b << 20 | b >>> 12) + c | 0; a += (b ^ c ^ d) + k[5] - 378558 | 0; a = (a << 4 | a >>> 28) + b | 0; d += (a ^ b ^ c) + k[8] - 2022574463 | 0; d = (d << 11 | d >>> 21) + a | 0; c += (d ^ a ^ b) + k[11] + 1839030562 | 0; c = (c << 16 | c >>> 16) + d | 0; b += (c ^ d ^ a) + k[14] - 35309556 | 0; b = (b << 23 | b >>> 9) + c | 0; a += (b ^ c ^ d) + k[1] - 1530992060 | 0; a = (a << 4 | a >>> 28) + b | 0; d += (a ^ b ^ c) + k[4] + 1272893353 | 0; d = (d << 11 | d >>> 21) + a | 0; c += (d ^ a ^ b) + k[7] - 155497632 | 0; c = (c << 16 | c >>> 16) + d | 0; b += (c ^ d ^ a) + k[10] - 1094730640 | 0; b = (b << 23 | b >>> 9) + c | 0; a += (b ^ c ^ d) + k[13] + 681279174 | 0; a = (a << 4 | a >>> 28) + b | 0; d += (a ^ b ^ c) + k[0] - 358537222 | 0; d = (d << 11 | d >>> 21) + a | 0; c += (d ^ a ^ b) + k[3] - 722521979 | 0; c = (c << 16 | c >>> 16) + d | 0; b += (c ^ d ^ a) + k[6] + 76029189 | 0; b = (b << 23 | b >>> 9) + c | 0; a += (b ^ c ^ d) + k[9] - 640364487 | 0; a = (a << 4 | a >>> 28) + b | 0; d += (a ^ b ^ c) + k[12] - 421815835 | 0; d = (d << 11 | d >>> 21) + a | 0; c += (d ^ a ^ b) + k[15] + 530742520 | 0; c = (c << 16 | c >>> 16) + d | 0; b += (c ^ d ^ a) + k[2] - 995338651 | 0; b = (b << 23 | b >>> 9) + c | 0; a += (c ^ (b | ~d)) + k[0] - 198630844 | 0; a = (a << 6 | a >>> 26) + b | 0; d += (b ^ (a | ~c)) + k[7] + 1126891415 | 0; d = (d << 10 | d >>> 22) + a | 0; c += (a ^ (d | ~b)) + k[14] - 1416354905 | 0; c = (c << 15 | c >>> 17) + d | 0; b += (d ^ (c | ~a)) + k[5] - 57434055 | 0; b = (b << 21 | b >>> 11) + c | 0; a += (c ^ (b | ~d)) + k[12] + 1700485571 | 0; a = (a << 6 | a >>> 26) + b | 0; d += (b ^ (a | ~c)) + k[3] - 1894986606 | 0; d = (d << 10 | d >>> 22) + a | 0; c += (a ^ (d | ~b)) + k[10] - 1051523 | 0; c = (c << 15 | c >>> 17) + d | 0; b += (d ^ (c | ~a)) + k[1] - 2054922799 | 0; b = (b << 21 | b >>> 11) + c | 0; a += (c ^ (b | ~d)) + k[8] + 1873313359 | 0; a = (a << 6 | a >>> 26) + b | 0; d += (b ^ (a | ~c)) + k[15] - 30611744 | 0; d = (d << 10 | d >>> 22) + a | 0; c += (a ^ (d | ~b)) + k[6] - 1560198380 | 0; c = (c << 15 | c >>> 17) + d | 0; b += (d ^ (c | ~a)) + k[13] + 1309151649 | 0; b = (b << 21 | b >>> 11) + c | 0; a += (c ^ (b | ~d)) + k[4] - 145523070 | 0; a = (a << 6 | a >>> 26) + b | 0; d += (b ^ (a | ~c)) + k[11] - 1120210379 | 0; d = (d << 10 | d >>> 22) + a | 0; c += (a ^ (d | ~b)) + k[2] + 718787259 | 0; c = (c << 15 | c >>> 17) + d | 0; b += (d ^ (c | ~a)) + k[9] - 343485551 | 0; b = (b << 21 | b >>> 11) + c | 0; x[0] = a + x[0] | 0; x[1] = b + x[1] | 0; x[2] = c + x[2] | 0; x[3] = d + x[3] | 0 } function md5blk(s) { var md5blks = [], i; for (i = 0; i < 64; i += 4) { md5blks[i >> 2] = s.charCodeAt(i) + (s.charCodeAt(i + 1) << 8) + (s.charCodeAt(i + 2) << 16) + (s.charCodeAt(i + 3) << 24) } return md5blks } function md5blk_array(a) { var md5blks = [], i; for (i = 0; i < 64; i += 4) { md5blks[i >> 2] = a[i] + (a[i + 1] << 8) + (a[i + 2] << 16) + (a[i + 3] << 24) } return md5blks } function md51(s) { var n = s.length, state = [1732584193, -271733879, -1732584194, 271733878], i, length, tail, tmp, lo, hi; for (i = 64; i <= n; i += 64) { md5cycle(state, md5blk(s.substring(i - 64, i))) } s = s.substring(i - 64); length = s.length; tail = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]; for (i = 0; i < length; i += 1) { tail[i >> 2] |= s.charCodeAt(i) << (i % 4 << 3) } tail[i >> 2] |= 128 << (i % 4 << 3); if (i > 55) { md5cycle(state, tail); for (i = 0; i < 16; i += 1) { tail[i] = 0 } } tmp = n * 8; tmp = tmp.toString(16).match(/(.*?)(.{0,8})$/); lo = parseInt(tmp[2], 16); hi = parseInt(tmp[1], 16) || 0; tail[14] = lo; tail[15] = hi; md5cycle(state, tail); return state } function md51_array(a) { var n = a.length, state = [1732584193, -271733879, -1732584194, 271733878], i, length, tail, tmp, lo, hi; for (i = 64; i <= n; i += 64) { md5cycle(state, md5blk_array(a.subarray(i - 64, i))) } a = i - 64 < n ? a.subarray(i - 64) : new Uint8Array(0); length = a.length; tail = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]; for (i = 0; i < length; i += 1) { tail[i >> 2] |= a[i] << (i % 4 << 3) } tail[i >> 2] |= 128 << (i % 4 << 3); if (i > 55) { md5cycle(state, tail); for (i = 0; i < 16; i += 1) { tail[i] = 0 } } tmp = n * 8; tmp = tmp.toString(16).match(/(.*?)(.{0,8})$/); lo = parseInt(tmp[2], 16); hi = parseInt(tmp[1], 16) || 0; tail[14] = lo; tail[15] = hi; md5cycle(state, tail); return state } function rhex(n) { var s = "", j; for (j = 0; j < 4; j += 1) { s += hex_chr[n >> j * 8 + 4 & 15] + hex_chr[n >> j * 8 & 15] } return s } function hex(x) { var i; for (i = 0; i < x.length; i += 1) { x[i] = rhex(x[i]) } return x.join("") } if (hex(md51("hello")) !== "5d41402abc4b2a76b9719d911017c592") { add32 = function (x, y) { var lsw = (x & 65535) + (y & 65535), msw = (x >> 16) + (y >> 16) + (lsw >> 16); return msw << 16 | lsw & 65535 } } if (typeof ArrayBuffer !== "undefined" && !ArrayBuffer.prototype.slice) { (function () { function clamp(val, length) { val = val | 0 || 0; if (val < 0) { return Math.max(val + length, 0) } return Math.min(val, length) } ArrayBuffer.prototype.slice = function (from, to) { var length = this.byteLength, begin = clamp(from, length), end = length, num, target, targetArray, sourceArray; if (to !== undefined) { end = clamp(to, length) } if (begin > end) { return new ArrayBuffer(0) } num = end - begin; target = new ArrayBuffer(num); targetArray = new Uint8Array(target); sourceArray = new Uint8Array(this, begin, num); targetArray.set(sourceArray); return target } })() } function toUtf8(str) { if (/[\u0080-\uFFFF]/.test(str)) { str = unescape(encodeURIComponent(str)) } return str } function utf8Str2ArrayBuffer(str, returnUInt8Array) { var length = str.length, buff = new ArrayBuffer(length), arr = new Uint8Array(buff), i; for (i = 0; i < length; i += 1) { arr[i] = str.charCodeAt(i) } return returnUInt8Array ? arr : buff } function arrayBuffer2Utf8Str(buff) { return String.fromCharCode.apply(null, new Uint8Array(buff)) } function concatenateArrayBuffers(first, second, returnUInt8Array) { var result = new Uint8Array(first.byteLength + second.byteLength); result.set(new Uint8Array(first)); result.set(new Uint8Array(second), first.byteLength); return returnUInt8Array ? result : result.buffer } function hexToBinaryString(hex) { var bytes = [], length = hex.length, x; for (x = 0; x < length - 1; x += 2) { bytes.push(parseInt(hex.substr(x, 2), 16)) } return String.fromCharCode.apply(String, bytes) } function SparkMD5() { this.reset() } SparkMD5.prototype.append = function (str) { this.appendBinary(toUtf8(str)); return this }; SparkMD5.prototype.appendBinary = function (contents) { this._buff += contents; this._length += contents.length; var length = this._buff.length, i; for (i = 64; i <= length; i += 64) { md5cycle(this._hash, md5blk(this._buff.substring(i - 64, i))) } this._buff = this._buff.substring(i - 64); return this }; SparkMD5.prototype.end = function (raw) { var buff = this._buff, length = buff.length, i, tail = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], ret; for (i = 0; i < length; i += 1) { tail[i >> 2] |= buff.charCodeAt(i) << (i % 4 << 3) } this._finish(tail, length); ret = hex(this._hash); if (raw) { ret = hexToBinaryString(ret) } this.reset(); return ret }; SparkMD5.prototype.reset = function () { this._buff = ""; this._length = 0; this._hash = [1732584193, -271733879, -1732584194, 271733878]; return this }; SparkMD5.prototype.getState = function () { return { buff: this._buff, length: this._length, hash: this._hash } }; SparkMD5.prototype.setState = function (state) { this._buff = state.buff; this._length = state.length; this._hash = state.hash; return this }; SparkMD5.prototype.destroy = function () { delete this._hash; delete this._buff; delete this._length }; SparkMD5.prototype._finish = function (tail, length) { var i = length, tmp, lo, hi; tail[i >> 2] |= 128 << (i % 4 << 3); if (i > 55) { md5cycle(this._hash, tail); for (i = 0; i < 16; i += 1) { tail[i] = 0 } } tmp = this._length * 8; tmp = tmp.toString(16).match(/(.*?)(.{0,8})$/); lo = parseInt(tmp[2], 16); hi = parseInt(tmp[1], 16) || 0; tail[14] = lo; tail[15] = hi; md5cycle(this._hash, tail) }; SparkMD5.hash = function (str, raw) { return SparkMD5.hashBinary(toUtf8(str), raw) }; SparkMD5.hashBinary = function (content, raw) { var hash = md51(content), ret = hex(hash); return raw ? hexToBinaryString(ret) : ret }; SparkMD5.ArrayBuffer = function () { this.reset() }; SparkMD5.ArrayBuffer.prototype.append = function (arr) { var buff = concatenateArrayBuffers(this._buff.buffer, arr, true), length = buff.length, i; this._length += arr.byteLength; for (i = 64; i <= length; i += 64) { md5cycle(this._hash, md5blk_array(buff.subarray(i - 64, i))) } this._buff = i - 64 < length ? new Uint8Array(buff.buffer.slice(i - 64)) : new Uint8Array(0); return this }; SparkMD5.ArrayBuffer.prototype.end = function (raw) { var buff = this._buff, length = buff.length, tail = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], i, ret; for (i = 0; i < length; i += 1) { tail[i >> 2] |= buff[i] << (i % 4 << 3) } this._finish(tail, length); ret = hex(this._hash); if (raw) { ret = hexToBinaryString(ret) } this.reset(); return ret }; SparkMD5.ArrayBuffer.prototype.reset = function () { this._buff = new Uint8Array(0); this._length = 0; this._hash = [1732584193, -271733879, -1732584194, 271733878]; return this }; SparkMD5.ArrayBuffer.prototype.getState = function () { var state = SparkMD5.prototype.getState.call(this); state.buff = arrayBuffer2Utf8Str(state.buff); return state }; SparkMD5.ArrayBuffer.prototype.setState = function (state) { state.buff = utf8Str2ArrayBuffer(state.buff, true); return SparkMD5.prototype.setState.call(this, state) }; SparkMD5.ArrayBuffer.prototype.destroy = SparkMD5.prototype.destroy; SparkMD5.ArrayBuffer.prototype._finish = SparkMD5.prototype._finish; SparkMD5.ArrayBuffer.hash = function (arr, raw) { var hash = md51_array(new Uint8Array(arr)), ret = hex(hash); return raw ? hexToBinaryString(ret) : ret }; return SparkMD5 });
(function (i) { i.toast = function (b) { i("#toast-container").length || (i("body").prepend("<div id=\"toast-container\" aria-live=\"polite\" aria-atomic=\"true\"></div>"), i("#toast-container").append("<div id=\"toast-wrapper\"></div>"), i("body").on("hidden.bs.toast", ".toast", function () { i(this).remove() })); var j = "toast-" + (i(".toast").length + 1), h = "", o = h = "", s = "text-muted", t = "", u = b.title || "Notice!", q = b.subtitle || "", r = b.content || "", p = b.delay || -1, k = b.img, d = b.pause_on_hover || !1, v = !1, w = ""; switch (b.type || "info") { case "info": h = "bg-info", t = s = o = "text-white"; break; case "success": h = "bg-success", t = s = o = "text-white"; break; case "warning": case "warn": h = "bg-warning", t = s = o = "text-white"; break; case "error": case "danger": h = "bg-danger", t = s = o = "text-white"; }!1 === d ? w = -1 === p ? "data-autohide=\"false\"" : "data-delay=\"" + p + "\"" : (b = Math.floor(Date.now() / 1E3) + p / 1E3, w = "data-autohide=\"false\"", d = "data-hide-timestamp=\"" + b + "\""), h = "<div id=\"" + j + "\" class=\"toast\" role=\"alert\" aria-live=\"assertive\" aria-atomic=\"true\" " + w + " " + d + ">" + ("<div class=\"toast-header " + h + " " + o + "\">"), "undefined" != typeof k && (h += "<img src=\"" + k.src + "\" class=\"" + (k["class"] || "") + " mr-2\" alt=\"" + (k.alt || "Image") + "\" " + ("undefined" == typeof k.title ? "" : "data-toggle=\"tooltip\" title=\"" + k.title + "\"") + ">"), h = h + ("<strong class=\"mr-auto\">" + u + "</strong>") + ("<small class=\"" + s + "\">" + q + "</small>"), h += "<button type=\"button\" class=\"ml-2 mb-1 close\" data-dismiss=\"toast\" aria-label=\"Close\">", h += "<span aria-hidden=\"true\" class=\"" + t + "\">&times;</span>", h += "</button>", h += "</div>", "" !== r && (h += "<div class=\"toast-body\">", h += r, h += "</div>"), h += "</div>", i("#toast-wrapper").append(h), i("#toast-wrapper .toast:last").toast("show"), !1 !== d && (setTimeout(function () { v || i("#" + j).toast("hide") }, p), i(document).on("mouseover", "#" + j, function () { v = !0 }), i(document).on("mouseleave", "#" + j, function () { var b = Math.floor(Date.now() / 1E3), a = parseInt(i(this).data("hide-timestamp")); v = !1, b >= a && i(this).toast("hide") })) } })(jQuery);

var List_Devices = "{\"DPM\":{\"name\":\"Micro_DPM380\",\"device\":\"Đồng hồ DPM\",\"protocol\":\"RTU\",\"ss\":0},\"MFM\":{\"name\":\"Selec_MFM_383\",\"device\":\"Đồng hồ MFM\",\"protocol\":\"RTU\",\"ss\":0},\"VSE3T\":{\"name\":\"Vinasino_VSE3T_5B\",\"device\":\"Đồng hồ Vinasino VSE3T\",\"protocol\":\"IEC\",\"ss\":0},\"VSE1T\":{\"name\":\"Vinasino_VSE1T_10100B\",\"device\":\"Đồng hồ Vinasino VSE1T\",\"protocol\":\"IEC\",\"ss\":0},\"SolarBK\":{\"name\":\"SolarBK_Grid_Tie\",\"device\":\"Inverter SolarBK Grid Tie\",\"protocol\":\"RTU\",\"ss\":0},\"SolarBKHybrid\":{\"name\":\"SolarBK_Hybrid\",\"device\":\"Inverter SolarBK Hybrid\",\"protocol\":\"RTU\",\"ss\":0},\"SMAGrid\":{\"name\":\"SMA_Grid_Tie\",\"device\":\"Inverter SMA Grid Tie\",\"protocol\":\"TCP\",\"ss\":1},\"SMABattery\":{\"name\":\"SMA_Battery_Inverter\",\"device\":\"Inverter SMA SunnyIsland\",\"protocol\":\"TCP\",\"ss\":0},\"SMASolidQ\":{\"name\":\"SMA_SolidQ\",\"device\":\"Inverter SMA SolidQ50/SolidQ60\",\"protocol\":\"RTU\",\"ss\":0},\"ABBGrid\":{\"name\":\"ABB_Grid_Tie\",\"device\":\"Inverter ABB Grid Tie\",\"protocol\":\"RTU\",\"ss\":1},\"Fronius\":{\"name\":\"Fronius_Grid_Tie\",\"device\":\"Inverter Fronius Grid Tie\",\"protocol\":\"RTU\",\"ss\":1},\"SolarEdge\":{\"name\":\"SolarEdge_Grid_Tie\",\"device\":\"Inverter SolarEdge Grid Tie\",\"protocol\":\"TCP\",\"ss\":1},\"Sungrow\":{\"name\":\"Sungrow_Grid_Tie\",\"device\":\"Inverter Sungrow Grid Tie\",\"protocol\":\"RTU\",\"ss\":0},\"Huawei\":{\"name\":\"Huawei_Grid_Tie\",\"device\":\"Inverter Huawei Grid Tie V2\",\"protocol\":\"RTU\",\"ss\":0},\"HuaweiV3\":{\"name\":\"Huawei_Grid_Tie_V3\",\"device\":\"Inverter Huawei Grid Tie V3\",\"protocol\":\"RTU\",\"ss\":0},\"GrowattTL3X\":{\"name\":\"Growatt_TL3X\",\"device\":\"Inverter Growatt TL3-X/TL-X\",\"protocol\":\"RTU\",\"ss\":0},\"KippZonen\":{\"name\":\"Kipp_and_Zonen_Solar_Radiation\",\"device\":\"Cảm biến bức xạ Kipp&Zonen\",\"protocol\":\"RTU\",\"ss\":0},\"IMT\":{\"name\":\"IMT_Solar_Radiation\",\"device\":\"Cảm biến bức xạ IMT\",\"protocol\":\"RTU\",\"ss\":0},\"SWH_System\":{\"name\":\"SWH_System\",\"device\":\"Hệ thống MNN QMCN\",\"protocol\":\"RTU\",\"ss\":0}}";
var IP_REGEXP = /^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$|^(([a-zA-Z]|[a-zA-Z][a-zA-Z0-9\-]*[a-zA-Z0-9])\.)*([A-Za-z]|[A-Za-z][A-Za-z0-9\-]*[A-Za-z0-9])$|^\s*((([0-9A-Fa-f]{1,4}:){7}([0-9A-Fa-f]{1,4}|:))|(([0-9A-Fa-f]{1,4}:){6}(:[0-9A-Fa-f]{1,4}|((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){5}(((:[0-9A-Fa-f]{1,4}){1,2})|:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){4}(((:[0-9A-Fa-f]{1,4}){1,3})|((:[0-9A-Fa-f]{1,4})?:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){3}(((:[0-9A-Fa-f]{1,4}){1,4})|((:[0-9A-Fa-f]{1,4}){0,2}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){2}(((:[0-9A-Fa-f]{1,4}){1,5})|((:[0-9A-Fa-f]{1,4}){0,3}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){1}(((:[0-9A-Fa-f]{1,4}){1,6})|((:[0-9A-Fa-f]{1,4}){0,4}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(:(((:[0-9A-Fa-f]{1,4}){1,7})|((:[0-9A-Fa-f]{1,4}){0,5}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:)))(%.+)?\s*$/;
var Internet_List = [];

var SSID_selected = ["", 0];
var lang_list = {
  "vi": {
    "btn-set": "Cài đặt",
    "btn-add": "Thêm",
    "btn-scan": "Quét",
    "btn-update": "Cập nhật",
    "btn-confirm": "Xác nhận",
    "btn-del": "Xóa",
    "lb-serial": "Cài đặt số Serial Number",

    "lb-lang": "Ngôn ngữ",
    "adminnav": "ADMIN",
    "hwtest": "KIỂM TRA PHẦN CỨNG",
    "conectset": "CÀI ĐẶT KẾT NỐI",
    "devicecomm": "CÀI ĐẶT GIAO TIẾP",
    "devicestatus": "THÔNG TIN THIẾT BỊ",
    "btnlogout": "Đăng xuất",


    // tab setting  
    "lb-devset-hd": "Cài đặt thiết bị giao tiếp",
    "lb-devset-chose": "Chọn thiết bị",
    // tab connection 
    "lb-connect-hd": "Cài đặt kết nối",
    "lb-wifiEth-hd": "Cài đặt Wifi/Ethernet",
    "lb-3g-hd": "<h5>Cài đặt 3G</h5>",
    "lb-wifi-hd": "WiFi",
    "lb-Eth-hd": "Ethernet",
    "lb-connect-wifi": "Điểm truy cập AP:",
    "lb-connect-pwd": "Mật khẩu:",
    "lb-connect-list": "Danh sách mạng WiFi khả dụng",
    "lb-eth-ip": "Địa chỉ IP:",
    "lb-eth-staticip": "IP tĩnh",
    "lb-eth-dynamicip": "IP động",
    "device-info": "Thông tin thiết bị",
    "dev-setEth": "Cài đặt Ethernet",

    "lb-guide-header": "Hướng dẫn cài đặt nhanh",

    "lb-wifi-status": "TRẠNG THÁI KẾT NỐI",
    "lb-wifi-netavl": "MẠNG CÓ SẴN",
    "lb-device-col": "Tên thiết bị",
    "txt-device-status-hd": "Thông tin thiết bị"

  },
  "en": {
    "btn-set": "Set",
    "btn-add": "Add",
    "btn-scan": "Scan",
    "btn-update": "Update",
    "btn-confirm": "Confirm",
    "btn-del": "Delete",
    "lb-serial": "Serial Number setting",

    "lb-lang": "Language",
    "adminnav": "ADMIN",
    "hwtest": "HARDWARE TESTING",
    "conectset": "CONNECTION SETTING",
    "devicecomm": "COMMUNICATION SETTING",
    "devicestatus": "DEVICE STATUS",
    "btnlogout": "Log out",

    // tab setting
    "lb-devset-hd": "RS485 Devices",
    "lb-devset-chose": "Choose device",
    // tab connection 
    "lb-connect-hd": "Connection setting",
    "lb-wifiEth-hd": "Wifi/Ethernet setting",
    "lb-3g-hd": "3G setting",
    "lb-wifi-hd": "WiFi",
    "lb-Eth-hd": "Ethernet",
    "lb-connect-wifi": "WiFi name:",
    "lb-connect-pwd": "Password: ",
    "lb-connect-list": "List of available WiFi networks",
    "lb-eth-ip": "IP address:",
    "lb-eth-staticip": "Dynamic IP",
    "lb-eth-dynamicip": "Static IP",
    "device-info": "Device information",
    "dev-setEth": "Ethernet setting",

    "lb-guide-header": "Quick setting guide",

    "lb-wifi-status": "CONNECTION STATUS",
    "lb-wifi-netavl": "NETWORKS AVAILABLE",
    "lb-device-col": "Device name",


    "txt-device-status-hd": "Device status"

  }
};
function sec2time(sec) {
  var time = "";
  if (sec > 31536000) {
    time += parseInt(Number(sec / 31536000)) + " năm ";
    sec = sec % 31536000;
  }
  if (sec > 86400) {
    time += parseInt(Number(sec / 86400)) + " ngày ";
    sec = sec % 86400;
  }
  if (sec > 3600) {
    time += parseInt(Number(sec / 3600)) + " giờ ";
    sec = sec % 3600;
  }
  if (sec > 60) {
    time += parseInt(Number(sec / 60)) + " phút";
  } else time = " 0 phút";
  return time;
}

/*********************TEMPLATE FUNCTION********************************/

function lang_change() {
  var lang = document.getElementById('lang_select').value;
  var guide = document.getElementsByClassName('guide-content');
  var guide_lang;
  for (i = 0; i < guide.length; i++) {
    if (guide[i].style.display == "block") {
      guide_lang = guide[i].children;
    }
  }
  if (lang == 'vi') {
    guide_lang[0].style.display = "block";
    guide_lang[1].style.display = "none";

    var keys = Object.keys(lang_list.vi);
    for (j = 0; j < keys.length; j++)
      if (document.getElementById(keys[j]))
        document.getElementById(keys[j]).innerHTML = lang_list.vi[keys[j]];
      else if (document.getElementsByClassName(keys[j])) {
        var parent = document.getElementsByClassName(keys[j]);
        for (i = 0; i < parent.length; i++)
          parent[i].innerHTML = lang_list.vi[keys[j]];
      }
  } else if (lang == 'en') {
    guide_lang[0].style.display = "none";
    guide_lang[1].style.display = "block";

    var keys = Object.keys(lang_list.en);
    for (j = 0; j < keys.length; j++)
      if (document.getElementById(keys[j]))
        document.getElementById(keys[j]).innerHTML = lang_list.en[keys[j]];
      else if (document.getElementsByClassName(keys[j])) {
        var parent = document.getElementsByClassName(keys[j]);
        for (i = 0; i < parent.length; i++)
          parent[i].innerHTML = lang_list.en[keys[j]];
      }
  }
}
function openNav() {
  if (document.getElementById("mySidebar").offsetWidth == 0) {
    document.getElementById("mySidebar").style.width = "250px";
    if (screen.width > 480)
      document.getElementById("container").style.marginLeft = "250px";
  }
  else {
    document.getElementById("mySidebar").style.width = "0";
    if (screen.width > 480)
      document.getElementById("container").style.marginLeft = "0";
  }
}

function closeNav() {
  document.getElementById("mySidebar").style.width = "0";
  document.getElementById("container").style.marginLeft = "0";
}
function navState(evt, cityName) {
  var i, tabcontent, tablinks;
  tabcontent = document.getElementsByClassName("navcontent");
  for (i = 0; i < tabcontent.length; i++) {
    tabcontent[i].style.display = "none";
  }
  tablinks = document.getElementsByClassName("tablinks");
  for (i = 0; i < tablinks.length; i++) {
    tablinks[i].className = tablinks[i].className.replace(" active", "");
  }
  document.getElementById(cityName).style.display = "block";

  var guide = document.getElementsByClassName("guide-content");
  for (i = 0; i < guide.length; i++) {
    if (guide[i].id == "guide-" + cityName) {
      guide[i].style.display = "block";
      if (document.getElementById('lang_select').value == "vi") {
        document.getElementById(guide[i].id + '-vi').style.display = "block";
        document.getElementById(guide[i].id + '-en').style.display = "none";
      } else {
        document.getElementById(guide[i].id + '-vi').style.display = "none";
        document.getElementById(guide[i].id + '-en').style.display = "block";
      }
    } else {
      guide[i].style.display = "none";
    }
  }

  evt.currentTarget.className += " active";
}

function openGuide() {
  var guide = document.getElementById('guide');
  if (guide.style.display == "block") {
    guide.style.display = "none";
  } else {
    guide.style.display = "block"
  }
}

function openModal(content, btn = "", func = "") {
  document.getElementById('modalP').innerHTML = content;
  $('#myModal').modal('show');
  if (btn != "") {
    document.getElementById('btnModal').style.visibility = 'visible';
    document.getElementById('btnModal').innerHTML = btn;
    if (func != "") {
      document.getElementById('btnModal').onclick = func;
    }
  } else {
    document.getElementById('btnModal').style.visibility = 'hidden';
  }
}

document.getElementById("customFile").onchange = function () {
  console.log(this.value);
  // document.getElementById("abcd").classList.add("selected");
  document.getElementById("file-label").innerHTML = this.value.replace(/^.*(\\|\/|\:)/, '');
};
/* */

function duplicate(coppy_id, id) {
  var original = document.getElementById(coppy_id);
  var clone = original.cloneNode(true);
  clone.id = id;
  original.parentNode.appendChild(clone);
}

/**********************************************************************/
/******************************FUNCTION********************************/
function int2ip(ipInt) {
  return ((ipInt >>> 24) + '.' + (ipInt >> 16 & 255) + '.' + (ipInt >> 8 & 255) + '.' + (ipInt & 255));
}
function ip2int(ip) {
  return ip.split('.').reduce(function (ipInt, octet) { return (ipInt << 8) + parseInt(octet, 10) }, 0) >>> 0;
}
function matchExpression(str) {
  var rgularExp = {
    containsNumber: /\d+/,
    containsAlphabet: /[a-zA-Z]/,

    onlyLetters: /^[A-Za-z]+$/,
    onlyNumbers: /^[0-9]+$/,
    onlyMixOfAlphaNumeric: /^([0-9]+[a-zA-Z]+|[a-zA-Z]+[0-9]+)[0-9a-zA-Z]*$/
  }
  return rgularExp.onlyNumbers.test(str);
}
function onRowClick(tableId, callback) {
  var table = document.getElementById(tableId),
    rows = table.getElementsByTagName("tr"), i;
  for (i = 0; i < rows.length; i++) {
    table.rows[i].onclick = function (row) {
      return function () {
        callback(row);
      };
    }(table.rows[i]);
  }
};

function sort_list(list, max) {
  var list_device = [];
  for (i = 0; i < max; i++) {
    for (j = 0; j < list.length - 1; j++) {
      if (list[j + 1] == i) {
        list_device.push(list[j]);
        list_device.push(list[j + 1]);
      }
      j++;
    }
  }
  return list_device;
}

function sort_list_device(list, max_device) {
  var list_device = [];
  for (i = 0; i < max_device; i++) {
    for (j = 0; j < list.length; j++) {
      if (list[j][1] == i) {
        list_device.push(list[j]);
      }
    }
  }
  return list_device;
}

function List_Device_Name() {
  var name = [];
  var js = JSON.parse(List_Devices);
  for (var k in js)
    name.push(js[k].device);
  return name;
}

function List_Device_Value() {
  var name = [];
  var js = JSON.parse(List_Devices);
  for (var k in js)
    name.push(js[k].name);
  return name;
}

function validate_eth_form(_arr_ip) {
  if (!IP_REGEXP.test(_arr_ip[0][0] + "." + _arr_ip[0][1] + "." +
    _arr_ip[0][2] + "." + _arr_ip[0][3])) {
    openModal("Địa chỉ IP chưa chính xác")
    return false;
  }
  if (!IP_REGEXP.test(_arr_ip[1][0] + "." + _arr_ip[1][1] + "." +
    _arr_ip[1][2] + "." + _arr_ip[1][3])) {
    openModal("Địa chỉ subnet chưa chính xác")
    return false;
  }
  if (!IP_REGEXP.test(_arr_ip[2][0] + "." + _arr_ip[2][1] + "." +
    _arr_ip[2][2] + "." + _arr_ip[2][3])) {
    openModal("Địa chỉ gateway chưa chính xác")
    return false;
  }
  return true;
}

function validate_device_form(_type) {
  console.log(_type)
  if (document.getElementById("sel_rs485_mode").value == 0 && _type != "TCP") {
    openModal("RS485 Mode Slave")
    return;
  }
  if (document.getElementById('rs485-dev-id').value == "" && _type != "IEC") {
    openModal("ID của thiết bị chưa chính xác!");
    return false;
  }
  if (_type == "TCP") {
    var parent = document.getElementById('rs485-dev-ip').children;
    if (!IP_REGEXP.test(parent[0].value + "." + parent[2].value + "." +
      parent[4].value + "." + parent[6].value)) {
      openModal("Địa chỉ IP chưa chính xác")
      return false;
    }
    if (document.getElementById('rs485-dev-port').value == "") {
      openModal("Port chưa chính xác!");
      return false;
    }
  } else if (_type == "IEC") {
    if (document.getElementById('rs485-dev-id').value == "") {
      openModal("ID của thiết bị chưa chính xác!");
      return;
    }
  }
  return true;
}

//
// 1102
//

function Scan_SSID(text) {
  var parser = new DOMParser();
  var xml = parser.parseFromString(text.replace(/&/gi, ""), "text/xml");
  var i;
  var x = xml.getElementsByTagName("net");
  let List_tmp = [];
  for (i = 0; i < x.length; i++) {
    let tmp = [x[i].getElementsByTagName("ssid")[0].childNodes[0].nodeValue, x[i].getElementsByTagName("rssi")[0].childNodes[0].nodeValue];
    if (tmp[1] < 0 | tmp[1] > 100) {
      tmp[1] = 1;
    }
    if (SSID_selected[0] != tmp[0]) {
      List_tmp.push(tmp);
    } else {
      if (SSID_selected[1] == 0) {
        SSID_selected[1] = tmp[1];
      }
    }
  }
  Internet_List = [];
  Internet_List.push(["TRẠNG THÁI KẾT NỐI", "1"]);

  if (SSID_selected[0] != "") {
    Internet_List.push([SSID_selected[0], SSID_selected[1]]);
  } else {
    Internet_List.push(["Chưa kết nối!", "1"]);
  }
  Internet_List.push(["", "1"]);
  Internet_List.push(["MẠNG CÓ SẴN", "1"]);
  Internet_List = Internet_List.concat(List_tmp);

  ScanShow();
}

function addWifi(ssid) {
  var str = "<div><span id='lb-connect-wifi'>Điểm truy cập AP:</span> \
            <input class='form-control my-1' type='text' id='ssid'>\
            <span id='lb-connect-pwd'>Mật khẩu:</span><br>\
            <input class='form-control my-1' type='text' id='pass'>\
            </div>";
  openModal(str, "Cài đặt", SetWifi);
  if (ssid) {
    document.getElementById('ssid').value = ssid;
  }
}

function ModelSetWifi() {
  var str = "<div><span id='lb-connect-wifi'>Điểm truy cập AP:</span> \
          <input class='form-control my-1' type='text' id='ssid'>\
          <span id='lb-connect-pwd'>Mật khẩu:</span><br>\
          <input class='form-control my-1' type='text' id='pass'>\
          </div>";
  addWifi(SSID_selected[0]);
  SSID_selected[0] = "";
}

function ScanShow() {
  var Internet_Table = "";
  for (var i = 0; i < Internet_List.length; i++) {
    if (Internet_List[i][0] == "TRẠNG THÁI KẾT NỐI") {
      Internet_Table += "<tr><td class='text-left pl-2 border' colspan='2' style='background-color: #F2F2F2;' id='lb-wifi-status'>" + Internet_List[i][0] + "</td></tr>";
    } else if (Internet_List[i][0] == "MẠNG CÓ SẴN") {
      Internet_Table += "<tr><td class='text-left pl-2 border' colspan='2' style='background-color: #F2F2F2;' id='lb-wifi-netavl'>" + Internet_List[i][0] + "</td></tr>";
    }
    else if (Internet_List[i][0] == "Chưa kết nối!") {
      Internet_Table += "<tr><td class='' colspan='2'>" + Internet_List[i][0] + "</td>";
    }
    else if (Internet_List[i][0] == "") {
      Internet_Table += "<tr><td style='height:7px;' colspan='2' >" + Internet_List[i][0] + "</td></tr>";
    }
    else {
      Internet_Table += "<tr><td>" + Internet_List[i][0] + "</td><td style='text-align: center'>" + Internet_List[i][1] + "</td></tr>";
    }
  }
  document.getElementById("WIFI_SCAN").innerHTML = Internet_Table;

  onRowClick("WIFI_SCAN", function (row) {
    var table = document.getElementById("WIFI_SCAN"),
      rows = table.getElementsByTagName("tr"), i;
    for (i = 0; i < rows.length; i++) {
      table.rows[i].style.backgroundColor = ('transparent'); //0233
    }
    if (row.cells[0].innerHTML != "" &&
      row.cells[0].innerHTML != "TRẠNG THÁI KẾT NỐI" &&
      row.cells[0].innerHTML != "MẠNG CÓ SẴN" &&
      row.cells[0].innerHTML != "Chưa kết nối!") {
      row.style.backgroundColor = '#7f8fa6';
      SSID_selected[0] = row.cells[0].innerHTML;
    }
  });
}

function rs485_mode_change() {
  var mode = document.getElementById("sel_rs485_mode").value;
  document.getElementById("rs485-slave-content").style.visibility = Number(mode) ? "hidden" : "visible";
  document.getElementById("rs485-content").style.visibility = Number(mode) ? "visible" : "hidden";
  document.getElementById("rs485-content-tb").style.visibility = Number(mode) ? "visible" : "hidden";
}

function rs485_mode() {
  var mode = document.getElementById("sel_rs485_mode").value;
  return mode;
}

function add_row_rs485(_device_template) {
  var dev_n, id, ip = [], port = 0;
  var d = JSON.parse(List_Devices);
  var device_val = document.getElementById('List_Devices').value;
  dev_n = d[device_val].device

  // if(!validate_device_form(d[device_val].protocol)){
  //   return;
  // }

  var dev_list_name = List_Device_Value();
  var eth_dhcp = document.getElementById('radio-dynamicIP').checked;
  var _name_ip = ["eth-ip", "eth-subnet", "eth-gateway"], eth_ip = [];

  if (d[device_val].protocol == "TCP") {
    for (i = 0; i < 3; i++) {
      let _ip = [], parent = document.getElementById(_name_ip[i]).children;
      for (j = 0; j < parent.length; j += 2) {
        _ip.push(parent[j].value);
      }
      eth_ip.push(_ip);
    }
    if (!validate_eth_form(eth_ip))
      return;
  }

  if (d[device_val].protocol != "IEC") {
    id = document.getElementById('rs485-dev-id').value;
  }

  if (d[device_val].protocol == "TCP") {
    var parent = document.getElementById('rs485-dev-ip').children;
    for (i = 0; i < parent.length; i += 2) {
      ip.push(parent[i].value);
    }
    port = document.getElementById('rs485-dev-port').value;
  } else if (d[device_val].protocol == "IEC") {
    id = document.getElementById('rs485-dev-id').value;
    if (id.length < 12)
      id = "0".repeat(12 - id.length) + id;
  }

  var table = document.getElementById("t_ModbusList");
  var idx = 0;
  // for (idx = 0; idx < table.rows.length; idx++) {
  //   if (table.rows[idx].cells[2].innerHTML == id && table.rows[idx].cells[1].innerHTML == dev_n) {
  //     openModal("Thiết bị đã được cài đặt!");
  //     return;
  //   }
  // }
  if (table.rows.length >= 50) {
    openModal("Quá số lượng thiết bị cho phép.");
    return;
  }

  idx = 0;
  if (table.rows.length == 1) {
    idx = 1;
  } else if (table.rows.length > 1) {
    for (i = 0; i < table.rows.length; i++) {
      if (table.rows[i].cells[1].innerHTML == dev_n) {
        for (j = i; j < table.rows.length; j++) {
          if (table.rows[i].cells[1].innerHTML != table.rows[j].cells[1].innerHTML) {
            idx = j;
            break;
          }
        }
        break;
      }
    }
    if (idx == 0) {
      idx = table.rows.length;
    }
  }

  var vinasioinfo = [0, 0];
  if (d[device_val].name == "Vinasino_VSE3T_5B") {
    vinasioinfo[0] = document.getElementById('vnsn3TPrimary').value;
    vinasioinfo[1] = document.getElementById('vnsn3TSecondary').value;
    dev_n += "(" + vinasioinfo[0] + "/" + vinasioinfo[1] + ")";
  }

  var row = table.insertRow(idx);
  row.insertCell(0).innerHTML = "";
  row.insertCell(1).innerHTML = dev_n;
  row.insertCell(2).innerHTML = id;
  row.insertCell(3).innerHTML = ip.length != 0 ? (ip[0] + "." + ip[1] + "." + ip[2] + "." + ip[3]) : "";
  row.insertCell(4).innerHTML = port != 0 ? port : "";
  row.insertCell(5).innerHTML = "";

  var stt = 1; // update stt
  for (i = 1; i < table.rows.length; i++) {
    table.rows[i].cells[5].innerHTML = "<button  class=\"btn btn-danger btn-delete btn-sm my-1\" onclick=\"delete_row_rs485('t_ModbusList'," + i + ")\">Xóa</button>";
    table.rows[i].cells[0].innerHTML = stt;
    if ((i < table.rows.length - 1) && table.rows[i].cells[1].innerHTML != table.rows[i + 1].cells[1].innerHTML)
      stt = 1;
    else stt++;
  }
  var js = {
    "protocol": d[device_val].protocol,
    "id": id,
    "device": (dev_list_name.indexOf(d[device_val].name) + 1),
    "ss": d[device_val].ss,
    "port": port,
    "ip": ip2int(ip[0] + '.' + ip[1] + '.' + ip[2] + '.' + ip[3]).toString(),
    "eth_dhcp": Number(eth_dhcp),
    "eth_ip": (eth_dhcp && d[device_val].protocol == "TCP") ? ip2int(eth_ip[0][0] + '.' + eth_ip[0][1] + '.' + eth_ip[0][2] + '.' + eth_ip[0][3]) : 0,
    "eth_sunet": (eth_dhcp && d[device_val].protocol == "TCP") ? ip2int(eth_ip[1][0] + '.' + eth_ip[1][1] + '.' + eth_ip[1][2] + '.' + eth_ip[1][3]) : 0,
    "eth_gateway": (eth_dhcp && d[device_val].protocol == "TCP") ? ip2int(eth_ip[2][0] + '.' + eth_ip[2][1] + '.' + eth_ip[2][2] + '.' + eth_ip[2][3]) : 0,
    "vnsn3TPrimary": vinasioinfo[0],
    "vnsn3TSecondary": vinasioinfo[1]

  };
  document.getElementById('rs485-dev-id').value = "";

  let j_data = JSON.stringify(js)
  // if(d[device_val].name == "Vinasino_VSE3T_5B"){
  //   j_data += "&Primary=" + document.getElementById('vnsn3TPrimary').value;
  //   j_data += "&Secondary=" + document.getElementById('vnsn3TSecondary').value;
  // }
  //console.log(j_data);
  add_Device(j_data);
}

function delete_row_rs485(_table, _row) {
  var table = document.getElementById(_table);
  var device, id, ip, port, vinasioinfo = [0, 0];
  var arr_template = List_Device_Name();
  // '&' instead of '&amp'
  var device_name = table.rows[_row].cells[1].innerHTML.replace(/&amp;/g, '&');

  //console.log(device_name.match(/\(([^)]+)\)/));

  if (device_name.match(/\(([^)]+)\)/) != null) {
    vinasioinfo = device_name.match(/\(([^)]+)\)/)[1].split("/")
  }

  device_name = device_name.replace(/ *\([^)]*\) */g, ""); // remove primary/secondary
  device = arr_template.indexOf(device_name);

  id = table.rows[_row].cells[2].innerHTML;
  ip = ip2int(table.rows[_row].cells[3].innerHTML);
  port = table.rows[_row].cells[4].innerHTML;
  table.deleteRow(_row);
  var stt = 1;
  for (i = 1; i < table.rows.length; i++) {
    table.rows[i].cells[5].innerHTML = "<button class=\"btn btn-danger btn-delete btn-sm my-1\" onclick=\"delete_row_rs485('" + _table + "'," + i + ")\">Xóa</button>";
    table.rows[i].cells[0].innerHTML = stt;

    if ((i < table.rows.length - 1) && table.rows[i].cells[1].innerHTML != table.rows[i + 1].cells[1].innerHTML)
      stt = 1;
    else stt++;
  }
  var d = Object.keys(JSON.parse(List_Devices)).map(function (_) { return JSON.parse(List_Devices)[_]; });
  var js = {
    "protocol": d[device].protocol,
    "id": id,
    "device": (device + 1),
    "ss": d[device].ss,
    "port": port,
    "ip": ip,
    "vnsn3TPrimary": vinasioinfo[0],
    "vnsn3TSecondary": vinasioinfo[1]
  };

  remove_device(JSON.stringify(js));
}

/**********************************************************************/
/******************************REQUEST********************************/
// OTE request
function File_ota(input) {
  var md5;
  var fileReader = new FileReader(),
    file = document.getElementById(input).files[0];
  // event
  fileReader.onload = function (e) {
    if (file.size != e.target.result.byteLength) {
      // console.log("Browser reported success but could not read the file until the end");
    } else {
      // get md5 hash when file loaded
      md5 = SparkMD5.ArrayBuffer.hash(e.target.result);
    }
  };
  fileReader.onloadend = function () {
    console.log("md5: ", md5);
    var formData = new FormData();
    formData.append("md5", md5);
    formData.append("myFile", file);
    var xhr = new XMLHttpRequest();
    xhr.onload = function () {
      if (this.readyState == 4 && this.status == 200) {
        openModal("Update Firmware: " + this.responseText);
      }
    }
    // upload progress event
    xhr.upload.addEventListener("progress", function (evt) {
      if (evt.lengthComputable) {
        var per = evt.loaded / evt.total;
        document.getElementById('bar').style.width = Math.round(per * 100) + "%";
        document.getElementById('progresspercent').innerHTML = Math.round(per * 100) + "%";
      }
    }, false);
    xhr.open("POST", "/update");
    xhr.send(formData);

  };
  fileReader.readAsArrayBuffer(file);
}

//======================================================================
//case 0

// case 1 choseWiFi3G
function choseNetType(type, e) { // enWiFiEth, en3G
  var mes = "";                 //Number(e.checked)
  mes = "m=1&js={\"NetType\": " + type + "}"; // 1-3G /  0-ETH/wifi
  console.log(mes);
  http_post(mes, "Enable component err", "/c", 1);
}

// case 2
function enableComponent(component, e) {
  var mes = "m=2&js={\"" + component + "\": " + Number(e.checked) + "}";
  http_post(mes, "Enable component err", "/c", 2);
}

//case 3
function ScanButton() {
  http_post('m=3', "Scan Err", "/c", 3);
}

// case 4
function SetWifi() {
  var mes = "m=4&js={\"ssid\":\"";
  let ssid = document.getElementById("ssid").value;
  let pass = document.getElementById("pass").value;
  if (ssid == "") {
    openModal("Lỗi: Tên Wifi rỗng!");
    return;
  }
  if (pass.length < 8) {
    openModal("Lỗi: Mật khẩu Wifi phải lớn hơn 8 kí tự!");
    return;
  }
  mes += ssid + "\", \"pwd\":\"";
  mes += pass + "\"}";
  http_post(mes, "Set WIFI Err", "/c", 4);
}

// case 5 
function EthConfig() { // send eth config
  var DHCP = Number(document.getElementById('radio-dynamicIP').checked);
  var _name_ip = ["eth-ip", "eth-subnet", "eth-gateway"], eth_ip = [];
  for (i = 0; i < 3; i++) {
    let _ip = [], parent = document.getElementById(_name_ip[i]).children;
    console.log(parent);
    for (j = 0; j <= 6; j += 2) {

      _ip.push(parent[j].value);
    }
    console.log(_ip);
    eth_ip.push(_ip);
  }
  if (DHCP) {
    if (!validate_eth_form(eth_ip))
      return;
  }
  var js = {
    "DHCP": DHCP,
    "ip": ip2int(eth_ip[0][0] + '.' + eth_ip[0][1] + '.' + eth_ip[0][2] + '.' + eth_ip[0][3]),
    "subnet": ip2int(eth_ip[1][0] + '.' + eth_ip[1][1] + '.' + eth_ip[1][2] + '.' + eth_ip[1][3]),
    "gateway": ip2int(eth_ip[2][0] + '.' + eth_ip[2][1] + '.' + eth_ip[2][2] + '.' + eth_ip[2][3])
  }

  http_post("m=5&js=" + JSON.stringify(js), "Set WIFI Err", "/c", 5);
}

//case 6 add rs485 device
function add_Device(js) {
  http_post("m=6&js=" + js, "Add device Err", "/c", 6);
}

//case 7 remove device
function remove_device(js) {
  http_post("m=7&js=" + js, "Remove device Err", "/c", 7);
}

//case 8 set serial number 
function set_Serial() {
  var mes = "m=8&Serial=" + document.getElementById("SerialID").value;
  http_post(mes, "Login Err", "/c", 8);

}

//case 8 set serial number 
function add_Serial() {
  var str = "<div>\
            <label>Lưu ý: Sau khi cài đặt số serial, thiết bị tự động reset.</label>\
            </div>";
  openModal(str, "Chấp nhận", set_Serial);
}
// case 10 set application



// case 22 logout
function logout() {
  var mes = "m=22";
  http_post(mes, "Login Err", "/c", 22);
}
// case 23 // set rs485 mode //0344
function set_rs485_mode() {
  var mes = "m=23&RS485_Mode=";
  mes += document.getElementById("sel_rs485_mode").value;
  mes += "&ID=" + document.getElementById("rs485_slave_id").value;
  http_post(mes, "Login Err", "/c", 23);
}
// case 24 // set auto manual
function other_set_mode(t) {
  http_post("m=24&Mode=" + Number(!t.checked), "Enable component err", "/c", 24);
}

function control(command) {
  var mes = "m=9&" + command;
  http_post(mes, "Login Err", "/c", 9);
}
//======================================================================

/**********************************************************************/
/*****************************REPAIR PAGE******************************/

function EthernetTypeIP(_choseType, Type) {
  var choseType = document.getElementById(Type)
  if (_choseType != "") {
    choseType.style.pointerEvents = 'auto';
    choseType.style.opacity = 1;
  } else {
    choseType.style.pointerEvents = 'none';
    choseType.style.opacity = 0.4;
  }
}

function DeviceInfo() {
  var device_val = document.getElementById('List_Devices').value;
  var js = JSON.parse(List_Devices);
  console.log(js);
  if (js[device_val].protocol == "RTU") {
    document.getElementById('TCP').style.display = 'none';
    document.getElementById('iec').style.display = 'none';
    document.getElementById("rs485-dev-id").style.width = '70px';
  } else if (js[device_val].protocol == "TCP") {
    document.getElementById('TCP').style.display = 'block';
    document.getElementById('iec').style.display = 'none';
    document.getElementById("rs485-dev-id").style.width = '70px';
  } else if (js[device_val].protocol == "IEC") {
    document.getElementById('TCP').style.display = 'none';
    document.getElementById('iec').style.display = 'block';
    document.getElementById("rs485-dev-id").style.width = '150px';
  }
  if (js[device_val].name == "Vinasino_VSE3T_5B") {
    document.getElementById("Vinasino_VSE3T_5B").style.display = "block";
  } else {
    document.getElementById("Vinasino_VSE3T_5B").style.display = "none";
  }
}


function repair_page() {
  Scan_SSID("");

  var sel = document.getElementById("List_Devices");
  var js = JSON.parse(List_Devices);
  for (var k in js) {
    var opt = document.createElement('option');
    opt.value = k;
    opt.text = js[k].device;
    // console.log(k);
    sel.add(opt);
  }
}

function update_rs485(_arr, _table) {
  var table = document.getElementById(_table);
  var rowCount = table.rows.length;
  for (var i = rowCount - 1; i > 0; i--) {
    table.deleteRow(i);
  }

  arr_template = List_Device_Name();
  arr = sort_list_device(_arr, arr_template.length + 1);
  var stt = 1, k = 0, length = arr.length;

  for (r = 0; r < length; r++) {
    var row = table.insertRow(r + 1);
    if (r > 0 && arr[r][1] != arr[r - 1][1])
      stt = 1;

    row.insertCell(0).innerHTML = stt;
    if (arr[r][1] != 3) { // check device vinasio 3t
      row.insertCell(1).innerHTML = arr_template[arr[r][1] - 1];
    } else {
      row.insertCell(1).innerHTML = arr_template[arr[r][1] - 1] + " (" + arr[r][5] + "/" + arr[r][6] + ")";
    }

    row.insertCell(2).innerHTML = arr[r][2];
    row.insertCell(3).innerHTML = arr[r][4] == "0" ? "" : int2ip(arr[r][4]);
    row.insertCell(4).innerHTML = arr[r][3] == "0" ? "" : arr[r][3];
    row.insertCell(5).innerHTML = "<button  class=\"btn btn-danger btn-delete btn-sm my-1\" onclick=\"delete_row_rs485('" + _table + "'," + (r + 1) + ")\">Xóa</button>";

    stt++;
  }
}

function updateState(obj) {
  var wifiEthTab = document.getElementById('wifiEthTab');
  var gsmTab = document.getElementById('gsmTab');
  var elmID = document.getElementById("SerialID");
  if (elmID) {
    elmID.value = obj.info.Serial;
  }
  // wifi type
  document.getElementById('enableWiFi').checked = obj.connection.wifi.en;
  if (obj.connection.wifi.en) {
    document.getElementById("con-wifi-content").style.opacity = 1;
    document.getElementById("con-wifi-content").style.pointerEvents = "auto";
    SSID_selected[0] = obj.connection.wifi.ssid;
    SSID_selected[1] = obj.connection.wifi.rssi;
    // Scan_SSID("");
  } else {
    document.getElementById("con-wifi-content").style.opacity = 0.4;
    document.getElementById("con-wifi-content").style.pointerEvents = "none"; //
  }
  //ethernet type con-eth-content
  document.getElementById('enableEthernet').checked = obj.connection.eth.en;
  if (obj.connection.eth.en) {
    document.getElementById("con-eth-content").style.opacity = 1;
    document.getElementById("con-eth-content").style.pointerEvents = "auto"; //
    EthernetTypeIP(!obj.connection.eth.DHCP, 'static-ip-1');
    if (!obj.connection.eth.DHCP) {

      document.getElementById('radio-dynamicIP').checked = true;
    } else {
      document.getElementById('radio-staticIP').checked = true;
    }

    var input_ip = document.getElementById('eth-ip').children;
    var input_subnet = document.getElementById('eth-subnet').children;
    var input_gateway = document.getElementById('eth-gateway').children;

    for (i = 0, k = 0; i <= 6; i += 2, k++) {
      input_ip[i].value = (obj.connection.eth.ip >> (24 - k * 8)) & 255;
    }
    for (i = 0, k = 0; i <= 6; i += 2, k++) {
      input_subnet[i].value = (obj.connection.eth.subnet >> (24 - k * 8)) & 255;
    }
    for (i = 0, k = 0; i <= 6; i += 2, k++) {
      input_gateway[i].value = (obj.connection.eth.gateway >> (24 - k * 8)) & 255;
    }
  } else {
    document.getElementById("con-eth-content").style.opacity = 0.4;
    document.getElementById("con-eth-content").style.pointerEvents = "none"; //
  }

  //gsm type
  document.getElementById('enable3G').checked = obj.connection.gsm.en;

  // console.log(obj.device.list_device);
  update_rs485(JSON.parse(obj.device.list_device), "t_ModbusList");

  // info 
  document.getElementById("info-serial").innerHTML = obj.info.Serial;
  document.getElementById("info-resettime").innerHTML = obj.info.Reset;
  document.getElementById("info-lifetime").innerHTML = sec2time(obj.info.Lifetime);
  document.getElementById("info-lastresettime").innerHTML = new Date(Date.now() - obj.info.Runningtime * 1000).toLocaleString();
  let sdtotoal = (obj.info.sdsize / 1024).toFixed(1);
  let sdused = (obj.info.sdusedsize / 1024).toFixed(2);

  document.getElementById("info-sdsize").innerHTML = sdtotoal != 0 ? (sdtotoal + " GB  -- \tĐã sử dụng: " + sdused + "GB") : ("No SD");
  document.getElementById("info-ssoclast").innerHTML = obj.info.lastssoc != 0 ? new Date((obj.info.lastssoc + (new Date().getTimezoneOffset()) * 60) * 1000).toLocaleString() : "#";

  // info-wifi
  if (obj.connection.wifi.connected) {
    document.getElementById("info-wifi-stt").innerHTML = "Đã kết nối";
    document.getElementById("wifi-info-explain").style.display = "block";
    document.getElementById("info-wifi-ssid").innerHTML = obj.connection.wifi.ssid;
    document.getElementById("info-wifi-rssi").innerHTML = (obj.connection.wifi.rssi + "%");
  } else {
    document.getElementById("info-wifi-stt").innerHTML = "Chưa kết nối";
    document.getElementById("wifi-info-explain").style.display = "none";
  }
  //info-eth
  if (obj.connection.eth.en) {
    document.getElementById("info-eth-explain").style.display = "block";
    document.getElementById("info-eth-stt").innerHTML = "Đã kết nối";
    document.getElementById("info-eth-ip").innerHTML = int2ip(obj.connection.eth.ip);
    document.getElementById("info-eth-subnet").innerHTML = int2ip(obj.connection.eth.subnet);
    document.getElementById("info-eth-gw").innerHTML = int2ip(obj.connection.eth.gateway);
  } else {
    document.getElementById("info-eth-stt").innerHTML = "Chưa kết nối";
    document.getElementById("info-eth-explain").style.display = "none";
  }
  //info gsm
  if (obj.connection.gsm.en) {
    document.getElementById("info-gsm-explain").style.display = "block";
    document.getElementById("info-gsm-stt").innerHTML = "Đã kết nối";
    document.getElementById("info-gsm-ser").innerHTML = obj.connection.gsm.ser;
    document.getElementById("info-gsm-rssi").innerHTML = obj.connection.gsm.rssi;

  } else {
    document.getElementById("info-gsm-explain").style.display = "none";
    document.getElementById("info-gsm-stt").innerHTML = "Chưa kết nối";
  }
  document.getElementById("info-totaldevices").innerHTML = obj.info.totaldevice
  document.getElementById("info-totaldevicecon").innerHTML = obj.info.totaldevCon;

}
//
/**************************************************************/
/***************************FUNCTION***************************/
function http_post(mess, err, url, handle) {
  var xhttp = new XMLHttpRequest();
  xhttp.onload = function () {
    if (this.readyState == 4 && this.status == 200) {
      switch (handle) {
        case 4: {
          if (this.responseText != "") {
            openModal("Thành công!");
          } else {
            openModal("Thất bại!");
          }
        } break;
        case 1: case 2:
        // case ((handle>=5  && handle<=7 )?handle:-1):     //5-8 // 
        case 5:
        case ((handle >= 10 && handle <= 19) ? handle : -1):   //10-18
        case 24: case 99: {
          if (this.responseText != "") {
            var obj = JSON.parse(this.responseText);
            updateState(obj);
            if (handle != 99) {
              openModal("Thành công!");
            }

          } else {
            openModal("Thất bại!");
          }
        } break;
        case 3: {
          Scan_SSID(this.responseText);
        } break;
        case 6: case 7: { // add, remove device
          if (this.responseText != "") {
            var obj = JSON.parse(this.responseText);
            updateState(obj);
            if (obj.ret == 1) {
              openModal("Thành công!");
            } else {
              openModal("Thất bại, ID đã tồn tại!");
            }
          } else {
            openModal("Thất bại!");
          }
        } break;
        case 8: case 9: case 23: {
          if (this.responseText == "OK") {
            openModal("Thành công!");
          } else {
            openModal("Thất bại!");
          }
        } break;

        case 22: {
          document.cookie = "username=; expires=Thu, 01 Jan 1970 00:00:00 UTC; path=/;";
          if (this.responseText = 'LOGOUT')
            location.replace("/");
        } break;
        case 100: {
          console.log(this.responseText);
          update_rs485(JSON.parse(this.responseText), "t_ModbusList");
        } break;
      }
    } else {
      openModal(err + this.status);
    }
  }
  xhttp.ontimeout = function () {
    openModal("An error occurred during the transaction");
  }
  if (handle == 2) {
    xhttp.timeout = 10000;
  } else {
    xhttp.timeout = 5000;
  }
  xhttp.open("POST", url, true);
  xhttp.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
  xhttp.send(mess);
}

function TestComponents(component, num, state) {
  var js = {
    "cm": 2, // hardware test
    "type": component,
    "NUM": num,
    "STATE": Number(state.checked)
  }
  console.log(js);
  ws.send(JSON.stringify(js));

}

function TestMode(state) {
  var mes = "{\"cm\":1,\"TEST_MODE\":" + Number(state.checked) + "}";
  if (!state.checked) {
    var t = document.getElementsByClassName('btnTest');
    for (let j = 0; j < t.length; j++) {
      t[j].checked = false;
    }
  }
  // if(!state.checked){
  //   var t = document.getElementsByClassName("btnTest");
  //   for(let i=0;i<t.length; i++){
  //     t[i].checked =  false;
  //   }
  // }
  console.log(mes);
  ws.send(mes);
}

var wifi_noti = 0;
var eth_noti = 0;
var gsm_noti = 0;

function Websocket() {
  if ("WebSocket" in window) {
    ws.onopen = function () {
      ws.send("open----------");
      get_list_directory("/");
    };
    ws.onmessage = function (evt) {
      let received_msg = evt.data;
      console.log(received_msg);

      var obj = JSON.parse(received_msg);
      switch (obj.cm) {
        case 3: { // net update
          switch (obj.type) {
            case 1: {// wifi
              if (obj.con == 0) {
                let table = document.getElementById("WIFI_SCAN");
                if (table.rows[1]) {
                  table.rows[1].cells[1].innerHTML = "";
                  table.rows[1].cells[0].colSpan = "2";
                  table.rows[1].cells[0].innerHTML = "Chưa kết nối!";
                }
                toast('info', "Đã ngắt kết nối WiFi!");
              }
              else if (obj.con == 1) {
                let table = document.getElementById("WIFI_SCAN");
                if (table.rows[1]) {
                  table.rows[1].cells[0].colSpan = "1";
                  table.rows[1].cells[0].innerHTML = obj.ssid;
                  if(!table.rows[1].cells[1]){
                    table.rows[1].insertCell(1); //map(rssi, (-90), (-30), 0, 100);
                  }
                  table.rows[1].cells[1].innerHTML = Math.round((100) * ((obj.rssi - -90) / (-30 - -90)));
                }
                toast('info', "Đã kết nối tới \"" + obj.ssid + " \"!");
              }
            } break;

            case 2: {
              if (obj.con != eth_noti) {
                eth_noti = obj.con;
                if (eth_noti == 1) {
                  toast('info', "Đã kết nối Ethernet");
                } else {
                  toast('info', "Đã ngắt kết nối Ethernet");
                }
              }
            } break;
            case 3: {
              if (obj.con == 1) {
                toast('info', "Đã kết nối GSM");
              } else if (obj.con == 2) {
                toast('info', "Kết nối tới GSM thất bại!");
                document.getElementById("enable3G").checked = false;
              } else {
                toast('info', "Đã ngắt kết nối GSM");
              }
            } break;
          }
        } break;
        case 4: {
          reset_directory(obj.path);
        } break;
        case 5: {
          document.getElementById("info-ssoclast").innerHTML = new Date((obj.time + (new Date().getTimezoneOffset()) * 60) * 1000).toLocaleString();
        } break;
      }
      ws.onclose = function () {
        toast("warning", "Mất kết nối");
      };
    }
  } else {
    toast("warning", "Mất kết nối");
    console.log("WebSocket NOT supported by your Browser!");
  }
}

// function set_time(){
//   var d = new Date();
//   var parent = document.getElementById('setting-time').children
//   var js = {
//     "DAY":    parent[4].value,
//     "MONTH":  parent[5].value,
//     "YEAR":   parent[6].value,
//     "WDAY":   d.getDay(),
//     "HOUR":   parent[0].value,
//     "MINUTE": parent[1].value,
//     "SEC":    d.getSeconds()

//   }

//   var mess="m=9&js="+JSON.stringify(js);
//   http_post(mess,"Set system err","/c",9);
// }
// var ws = new WebSocket("ws://localhost:81/"); //3
var ws = new WebSocket("ws://" + location.host + ":81/");
Websocket();

document.onreadystatechange = function () {
  var state = document.readyState;

  if (state == 'interactive') {
    document.getElementById('loading').style.display = "none";  /////////////////////////
    document.getElementById('container').style.display = "block";
  } else if (state == 'complete') {
    document.getElementById('loading').remove();
    document.getElementById('container').style.display = "block";
    // r0853
    repair_page();
    //
    if (window.innerWidth > 1300) {
      document.getElementById('openSidebar').click();
    }

    document.getElementById('conectset').click();
    // lang_change();

    let user = getCookie("username");
    switch (user) {
      case "YWRtaW4=": {
        document.getElementById('devicecomm').click();
        console.log("admin account");
      } break;
      case "bWFudQ==": {
        document.getElementById("devicecomm").remove();
        console.log("manufacturer account");
      } break;
      case "aW5zdA==": {
        document.getElementById("adminnav").remove();
        document.getElementById('devicecomm').click();
        console.log("installer account");
      } break;
      case "dXNlcg==": {
        document.getElementById("adminnav").remove();
        document.getElementById("devicecomm").remove(); //
        console.log("user account");
      } break;
    }
    http_post('m=99', "Scan Err", "/c", 99);
  }
}

// var isFocused = false;
// var IntervalTime = setInterval(myTimer, 1000);
// //
// function myTimer() {
//   var d = new Date();
//   if(!isFocused){
//     for(i = 0; i<1; i++){ // more parent
//       let parent = document.getElementById('setting-time');
//       if(parent){
//         let child = parent.children;
//         child[0].value = d.getHours();
//         child[1].value = d.getMinutes();
//         child[2].value = d.getSeconds();
//         child[4].value = d.getDate();
//         child[5].value =  d.getMonth()+1;
//         child[6].value = d.getFullYear();
//       }else{
//         console.log("timer not found");
//       }
//     }
//   }
// }

window.onload = function () {
  var parent = document.getElementsByClassName('time-field');
  for (i = 0; i < parent.length; i++) {
    parent[i].onfocus = function () {
      isFocused = true;
    };
    parent[i].onblur = function () {
      isFocused = false;
    };
  }
}

// var jsontemp = js.replace((/([\w]+)(:)/g), "\"$1\"$2");
/**********************************************************************/
function getCookie(cname) {
  var name = cname + "=";
  var decodedCookie = decodeURIComponent(document.cookie);
  var ca = decodedCookie.split(';');
  for (var i = 0; i < ca.length; i++) {
    var c = ca[i];
    while (c.charAt(0) == ' ') {
      c = c.substring(1);
    }
    if (c.indexOf(name) == 0) {
      return c.substring(name.length, c.length);
    }
  }
  return "";
}

function toast(type, notif) {
  $.toast({
    title: 'Thông báo',
    // subtitle: '11 mins ago',
    content: notif,
    type: type,
    delay: 5000
  });
}

// *****************************************

function test1(e) {
  let list = List_Device_Name;

  let s = JSON.stringify(list);
}

// *****************************************

function fileBack() {
  var back_path = document.getElementById("f_path").value;
  if (back_path != "") {
    back_path = back_path.substring(0, back_path.lastIndexOf("/"));
    if (back_path == "") {
      back_path = "/";
    }
  }
  get_list_directory(back_path);
}

function reset_directory(obj) {
  File_List = "";
  var back_path = document.getElementById("f_path").value;
  document.getElementById("t_file_body").innerHTML = File_List;

  if (back_path != "") {
    back_path = back_path.substring(0, back_path.lastIndexOf("/")); //0133
    if (back_path == "") {
      back_path = "/";
    }
  }
  console.log("BACK PATH -> " + back_path);

  // File_List += "<tr><td class='text-left pl-3 py-0 border align-middle'>/</td><td class='text-center border py-0'></td><td class='text-center border py-0'><button class='btn btn-info btn-sm' style='margin:2px 0px 2px;' onclick=\"get_list_directory('"+back_path+"')\">Back</button></td></tr>";

  if (obj) {
    let length = obj.array.length;

    for (let i = 0; i < length; i++) {
      var file_path = obj.array[i].substring(obj.array[i].lastIndexOf("/"), obj.array[i].length);
      File_List += "<tr><td class='text-left pl-3 py-0 border align-middle'>" + file_path;
      var request_path = obj.FILE == "/" ? file_path : obj.FILE + file_path;
      if (obj.array[i + 1] != "") {
        File_List += "</td><td class='text-center border py-0 align-middle'>" + FileSize(obj.array[i + 1]) + "</td>";
        File_List += "</td><td class='text-center border py-0'><button class='btn btn-success btn-sm my-1' onclick=\"reqest_file('" + request_path + "')\">Download</button></td></tr>";

      } else {
        File_List += "</td><td class='text-center border py-0'></td>";
        File_List += "</td><td class='text-center border py-0'><button class='btn btn-info btn-sm my-1' onclick=\"get_list_directory('" + request_path + "')\">Explore</button></td></tr>";
      }

      i++;
    }
  }

  document.getElementById("t_file_body").innerHTML = File_List;
}

reset_directory();

function get_list_directory(path) {
  let mess = "{\"cm\":4,\"path\":\"" + path + "\"}";
  console.log(mess);
  document.getElementById('f_path').value = path;
  ws.send(mess);
}

function reqest_file(path) {
  var mes = "path=" + path;
  window.location.href = "/f?" + mes;
}

function FileSize(size) {
  var i = Math.floor(Math.log(size) / Math.log(1024));
  return (size / Math.pow(1024, i)).toFixed(2) * 1 + ' ' + ['B', 'kB', 'MB', 'GB', 'TB'][i];
}

$(document).ready(function () {
  $('input.ip').on('input', function () {
    let s = $(this).val();
    $(this).val(s.replace(/\./g, ''));
  });
  // focus jump to the next input field when reaching maxlength
  $('input.ip').keyup(function (e) {
    console.log($(this))
    let b = false;
    var code = e.keyCode || e.which;
    if (code == 110 || code == 190) {
      let s = $(this).val();
      $(this).val(s.substring(s, s.length - 1));
      b = true;
    }
    if ($(this).val().length == $(this).attr("maxlength")) {
      b = true;
    }
    if (b && (code != 9)) {
      $(this).next().next().select();
      $(this).next().next().focus();
    }
  });
});

//--------- TEST FUNCTION


function bl_test_fnc(k) {
  // console.log(k.checked);
  js = "{\"mode\":" + (k.checked ? 1 : 0) + "}";
  console.log(js);
  http_post("m=98&js=" + js, "Remove device Err", "/c", 98);
}