dir c:\windows\system32\rpcnetp.*
dir c:\windows\syswow64\rpcnet*.*
tasklist /FI "IMAGENAME EQ rpcnetp.exe"
tasklist /FI "IMAGENAME EQ rpcnet.exe"
