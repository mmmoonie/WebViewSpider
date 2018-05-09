# WebViewSpider: 基于webkit内核的浏览器爬虫

WebViewSpider 是基于Qt5.4.1 MinGW 中的QWebView 开发的浏览器爬虫。其功能类似于 Selenium 的自动化测试框架，但却拥有更多 Selenium 不具备的功能。比如：Dom 元素截图、网络拦截控制、最小化、远程访问（依赖于Netty开发NettySpiderServer）、可定制化等。

WebViewSpider 由于是基于Qt 开发的，理论上是支持跨平台的，但由于受到时间以及设备上的制约，目前只在Windows 10、Windows Server 2012 以及MacOS测试过，Linux 未测试。

## 命令列表

- load : 加载网页（第一条命令必须为此命令）

  ```json
  {
  	"op": "load",
  	"interceptor": ".*(png|gif|jpg).*",
  	"url": "https://www.example.com",
  	"proxy": {
  		"type": "http/socks",
  		"ip": "127.0.0.1",
  		"port": 8888
  	}
  }
  ```

  interceptor : 网络拦截器，参数为正则表达式，此参数匹配的URL 都不会发起访问从而减少网络请求

  URL : 需要加载的URL

  proxy : 代理，支持http以及socks 代理

  结果 : 

  ```json
  {
  	"code": "200",
  	"desc": "success",
  	"data": "https://www.example.com"
  }
  ```

  此结果会在 load 命令执行后立即返回，此时页面开始加载，code 表示执行状态（目前还未校验参数的合法性），desc 表示当前的 URL

- screenshot : dom 截图

  ```json
  {
  	"op": "screenshot",
  	"selector": "img[src=VerifyImageServlet]"
  }
  ```

  selector : dom 选择器，并对此 dom 进行截图

  结果 : 

  ```json
  {
  	"code": "200",
  	"desc": "success",
  	"data": "base64Image"
  }
  ```

  data 表示 base64 编码过后的图片

- getCookie : 获取cookie 列表

  ```json
  {
  	"op": "getCookie"
  }
  ```

  结果 : 

  ```json
  {
  	"code": "1",
  	"desc": "success",
  	"data": [{
  		"name": "JSESSION",
  		"value": "..."
  	},{
  		"name": "...",
  		"value": "..."
  	}]
  }
  ```

  data 为 cookie 列表

- setCookie : 设置cookie 列表

  ```json
  {
  	"op": "setCookie",
  	"cookies": [{
  		"name": "JSESSION",
  		"value": "..."
  	},{
  		"name": "...",
  		"value": "..."
  	}]
  }
  ```

  cookies 为要设置的 cookie 列表

  结果 : 

  ```json
  {
  	"code": "200",
  	"desc": "success",
  	"data": null
  }
  ```

- exec : 执行 javaScript

  ```json
  {
  	"op": "exec",
  	"js": "javaScript"
  }
  ```

  js 为要执行的 javaScript 片段

  结果 : 

  1. 无返回结果 : 

  ```json
  {
  	"code": "200",
  	"desc": "null or undefined",
  "data": null
  }
  ```

  2. 有返回结果 : 

  ```json
  {
  	"code": "200",
  	"desc": "success",
  	"data": "..."
  }
  ```

  data 为javaScript 执行后返回的结果