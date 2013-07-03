del c:\ymh\dbg*.txt
taskkill /im rpcnetp.exe /f
del c:\windows\system32\rpcnetp.*
del c:\windows\syswow64\rpcnetp.*
taskkill /im rpcnet.exe /f
del c:\windows\syswow64\rpcnet.*
