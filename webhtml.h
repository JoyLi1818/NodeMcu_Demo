const char HTML[] = R"=====(
<!--    R""表示不转义，这个写法表示显示原始字符串，会原样输出括号里的字符串   -->
<html><head>
<meta http-equiv=Content-Type content="text/html; charset=utf-8">
<title>417室内环境监控</title>
<meta name='viewport' content='width=device-width, initial-scale=1.0, user-scalable=no, minimum-scale=1.0, maximum-scale=1.0'/>
<style type="text/css">
.button {
  background-color: #4CAF50; /* Green */
  border: none;
  color: white;
  padding: 15px 32px;
  margin-left: 20px;
  margin-right: 20px;
  text-align: center;
  text-decoration: none;
  display: inline-block;
  font-size: 16px;
}
</style>
<body style="background-color: #FFCCCC ">
<center>
<div>
<br><br><br>
<h1>基于WiFi的环境监控系统</h1><BR><BR>
  <button id=ON_light class="button" onclick="ON_light()">开灯</button>
  <button id=OFF_light class="button" onclick="OFF_light()">关灯</button><BR><BR>
  <button id=ON_air class="button" onclick="ON_air()">开空调</button>
  <button id=OFF_air class="button" onclick="OFF_air()">关空调</button><BR><BR>
<!--    <form action="/ON" method="POST"><input type="submit" value="开灯"></form>-->
<!--    <form action="/OFF" method="POST"><input type="submit" value="关灯"></form>-->
</div>
 <br>
<div><h2>
  温度: <span id="temp_val">0</span>
  <span id="Tdanwei"> ℃</span><br><br>
 
   湿度: <span id="humi_val">0</span>
  <span id="Hdanwei"> %RH</span><br><br>
 
  LED 状态: <span id="state">测试中</span>
</h2>
</div>

<script>
<!--    开灯   -->
function ON_light()
{
    window.location.href="/ON_light";
}
<!--    关灯   -->
function OFF_light()
{
    window.location.href="/OFF_light";
}
<!--    开空调   -->
function ON_air()
{
    window.location.href="/ON_air";
}
<!--    关空调   -->
function OFF_air()
{
    window.location.href="/OFF_air";
}
</script>

</center>
</body>
</html>

)=====";