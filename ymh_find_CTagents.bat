tasklist /FI "STATUS eq running" >> c:\ymh\dbg.txt
dir c:\windows\system32\rpcnetp.* >> c:\ymh\dbg.txt
dir c:\windows\syswow64\rpcnet*.* >> c:\ymh\dbg.txt
dir C:\windows\SysWow64\Upgrd.exe >> c:\ymh\dbg.txt
tasklist /FI "IMAGENAME EQ rpcnetp.exe" >> c:\ymh\dbg.txt
tasklist /FI "IMAGENAME EQ rpcnet.exe" >> c:\ymh\dbg.txt
