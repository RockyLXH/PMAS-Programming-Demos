#paramter for WINSCP session 
param (
	$remotePath = "/mnt/jffs/usr/",
    $fileName = "SIL_BASE.pexe"
)


#Get Refernce to file SILBASEParam.xml
$xdoc = [xml] (get-content ".\SILBASEParam.xml")
#Get PMAS IP
$PmasIP = $xdoc.root.PMAS_PARAM.IP 
#Get ONLINE mode (0 - work in offline mode compile and don't download. 1 - compile and download) 
$OnlineMode = $xdoc.root.PMAS_PARAM.ONLINE 
 Write-Host "OnlineMode = $OnlineMode"
##


#Option to get GMAS IP from user
#Write-Host "******************************'" -fore green
#$PmasIP = Read-Host  'Please enter PMAS IP:' 
#Write-Host "PMAS IP:'$PmasIP'" -fore green
#rite-Host "******************************'" -fore green
###

If ($OnlineMode -eq 1) {
#Download SIL_BASE.pexe using WinSCPnet.dll         
try
{
    # Load WinSCP .NET assembly
    #Add-Type -Path "WinSCPnet32.dll"
if ([Environment]::Is64BitProcess -ne [Environment]::Is64BitOperatingSystem)
{
	Add-Type -Path "C:\Program Files\WinSCP\WinSCPnet.dll"
}
else
{
	Add-Type -Path "C:\Program Files (x86)\WinSCP\WinSCPnet.dll"
}

    # Setup session options
    $sessionOptions = New-Object WinSCP.SessionOptions -Property @{
        Protocol = [WinSCP.Protocol]::SCP
		HostName = $PmasIP
        UserName = "root"
        Password = ""
    }
	
	$sessionOptions.GiveUpSecurityAndAcceptAnySshHostKey = "true" 
	
    $session = New-Object WinSCP.Session
 
    $transferOptions = New-Object WinSCP.TransferOptions
    $transferOptions.TransferMode = [WinSCP.TransferMode]::Binary
	$transferOptions.FilePermissions = New-Object WinSCP.FilePermissions
	$transferOptions.FilePermissions.Octal = "777"
	
    try
    {
        # Connect
        $session.Open($sessionOptions)
		
		# Kill SIL_BASE proceese 
		$dumpCommand = "killall -INT SIL_BASE.pexe"
        $session.ExecuteCommand($dumpCommand)
		#Let the porgram handle closing
		Sleep 3
		$session.PutFiles( (${SIL_WORKSPACE}script\ + $fileName), ($remotePath + $fileName), $False, $transferOptions)
		
		#Another option to Run SIL_BASE proceese not using telnet, need to check how it is close the connection 
		#$dumpCommand = "/mnt/jffs/usr/./SIL_BASE.pexe"
        #$session.ExecuteCommand($dumpCommand)
    }
    finally
    {
        # Disconnect, clean up
        $session.Dispose()
    }
 
   
}
catch [Exception]
{
    Write-Host "Error: $($_.Exception.Message)" -foregroundcolor "Red"
	Write-Host "Make sure WinSCP install and TARGET IP is SET" -foregroundcolor "Red"
	Write-Host "Press any key to continue ..." -foregroundcolor "Red"

	$x = $host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")

	
    exit
}

#Open telnet connection
#Kill SIL_BASE process
#Run SIL_BASE.pexe
function Connect-MyTelnet{
Param(
 [string] $IPAddress,
 [string] $Port,
 [string] $UserName,
 [string] $Password,
 [string] $cmdlistPath
)
    ## - Setting default values:
    if($port -eq $null){ $Port = "23"; };
    
## create vbscript file: MyTelnetSession.vbs
## - For Microsoft Telnet:
$MyVBScript = @"
set oShell = CreateObject("WScript.Shell")`r`n
oShell.run("Telnet")`r`n
WScript.Sleep 100`r`n
oShell.SendKeys("Open $IPAddress $Port")`r`n
WScript.Sleep 100`r`n
oShell.SendKeys("{Enter}")`r`n
WScript.Sleep 100`r`n
oShell.SendKeys("$UserName")`r`n
WScript.Sleep 100`r`n
oShell.SendKeys("{Enter}")`r`n
WScript.Sleep 100`r`n
oShell.SendKeys("killall -9 SIL_BASE.pexe")`r`n
WScript.Sleep 100`r`n
oShell.SendKeys("{Enter}")`r`n
WScript.Sleep 100`r`n
oShell.SendKeys("/mnt/jffs/usr/./SIL_BASE.pexe")`r`n
WScript.Sleep 100`r`n
oShell.SendKeys("{Enter}")`r`n
WScript.Sleep 100`r`n
"@;
 
  
    ## - Save and execute generated VBscript:
    $MYVBScript | Out-File -FilePath ${SIL_WORKSPACE}script\MyTelnet.vbs -Encoding ASCII;
    & {SIL_WORKSPACE}script\MyTelnet.vbs
}; Set-Alias ct Connect-MyTelnet;


taskkill /IM telnet.exe /F

Connect-MyTelnet `
    -IPAddress $PmasIP `
    -Port 23 -UserName root `
    -Password 'root' `
    ;
}