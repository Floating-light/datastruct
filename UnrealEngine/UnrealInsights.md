-tracehost=127.0.0.1 
-trace=cpu,gpu,frame,log,bookmark
-cpuprofilertrace
-loadtimetrace

UE4Editor.exe D:\Workspace\Project\u01\U01.uproject -game -WINDOWED -trace=cpu,gpu,frame,log,bookmark
先启动Unreal insights, 