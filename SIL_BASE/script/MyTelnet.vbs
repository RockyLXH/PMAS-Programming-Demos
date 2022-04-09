set oShell = CreateObject("WScript.Shell")

oShell.run("Telnet")

WScript.Sleep 100

oShell.SendKeys("Open 192.168.1.3 23")

WScript.Sleep 100

oShell.SendKeys("{Enter}")

WScript.Sleep 100

oShell.SendKeys("root")

WScript.Sleep 100

oShell.SendKeys("{Enter}")

WScript.Sleep 100

oShell.SendKeys("killall -9 SIL_BASE.pexe")

WScript.Sleep 100

oShell.SendKeys("{Enter}")

WScript.Sleep 100

oShell.SendKeys("/mnt/jffs/usr/./SIL_BASE.pexe")

WScript.Sleep 100

oShell.SendKeys("{Enter}")

WScript.Sleep 100

