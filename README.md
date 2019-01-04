# sofia-sip-demo
### about

本项目只是对sip 协议的 INVITE，BYE，ACK，等简单验证。uas采用了sipp，验证了REGISTER没有成功。

代码采用的是sofia-sip库来实现。

#### SIPp

SIPp是一个模拟sip协议的软件，可以对sip进行压力测试，对freeswitch进行压力测试。

源代码是开源的，需要在linux环境下编译，或者在cwgin下编译。

官方文档路径：http://sipp.sourceforge.net/doc/reference.html

简单的uas：./sipp -trace_calldebug -sn uas

brachs：./sipp -trace_calldebug -sn branchs

branchs只是简单验证了REGISTER 请求，SIPp可以收到也可以RESPONSE，但是不能继续接受INVITE请求。

-trace_calldebug只是打印debug信息，可以不加。

通过自定义的xml来启动：./sipp -trace_err -sf branchs.xml

#### sofia-sip

这是早年诺基亚实现的一个sip协议栈，功能丰富。网络层也是基于EventLoop来实现了Reactor。有自己的内存池，比如su_home_t。

代码非常多，接口丰富，功能强大，使用的门槛比较高。

官方文档路径：http://sofia-sip.sourceforge.net/development.html

#### sip

SIP(Session initial protocol) ，基于文本的类似 http的协议。

sip =: start-line

       headers
       
       CRLF
       
       [body]
       
start-line =: request-line | status-line

request-line =: method url sip-version

status-line =: sip-version status-code desc

body: 是sdp格式

method：INVITE，REGISTER，ACK，OPTIONS，BYE

参考：https://www.cnblogs.com/tester-l/p/6032245.html

rfc:rfc3261
