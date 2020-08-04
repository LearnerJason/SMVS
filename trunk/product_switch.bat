@echo off 

echo 请选择ART板卡型号:
echo.
echo 1 = PCI2312;
echo 2 = PCI2310;
echo 3 = PCI2330;
echo.

set /p d=请选择板卡类型: 

echo 请选择图检软件平台产品类型:
echo.
echo 1 = Crown Cap;
echo 2 = EOE;
echo 3 = CAN;
echo 4 = PET;
echo 5 = PCC;
echo 6 = AL_PLASTIC;
echo 7 = CAN_EOE;
echo 8 = JELLY;
echo 9 = CAN_OUTSIDE
echo 10 = NAIL
echo 11 = PLUG
echo 12 = HDPE
echo 13 = NEEDLE
echo 14 = RubberMat
echo 15 = BatteryShell
echo 16 = CapacitorShell
echo 17 = BottleCap
echo.

set /p c=请输入您的选择:  
if %c%==1 goto crown_cap  
if %c%==2 goto eoe 
if %c%==3 goto can 
if %c%==4 goto pet
if %c%==5 goto pcc
if %c%==6 goto al_plastic
if %c%==7 goto can_eoe
if %c%==8 goto jelly
if %c%==9 goto can_outside
if %c%==10 goto nail
if %c%==11 goto plug
if %c%==12 goto hdpe
if %c%==13 goto needle
if %c%==14 goto rubbermat
if %c%==15 goto BatteryShell
if %c%==16 goto Capacitor
if %c%==17 goto Bottle

:crown_cap 
E:\smvs_mcs\trunk\product_switcher.exe CROWN_PRODUCT %d%
if errorlevel 1 goto error
goto end

:eoe
E:\smvs_mcs\trunk\product_switcher.exe EOE_PRODUCT %d%
if errorlevel 1 goto error
goto end

:can
E:\smvs_mcs\trunk\product_switcher.exe CAN_PRODUCT %d%
if errorlevel 1 goto error
goto end

:can_eoe
E:\smvs_mcs\trunk\product_switcher.exe CAN_EOE_PRODUCT %d%
if errorlevel 1 goto error
goto end

:pcc
E:\smvs_mcs\trunk\product_switcher.exe PCC_PRODUCT %d%
if errorlevel 1 goto error
goto end

:pcc
E:\smvs_mcs\trunk\product_switcher.exe CAP_PRODUCT %d%
if errorlevel 1 goto error
goto end

:needle
E:\smvs_mcs\trunk\product_switcher.exe NEEDLE_PRODUCT %d%
if errorlevel 1 goto error
goto end

:pet
E:\smvs_mcs\trunk\product_switcher.exe PET_PRODUCT %d%
if errorlevel 1 goto error
goto end

:al_plastic
E:\smvs_mcs\trunk\product_switcher.exe AL_PLASTIC_PRODUCT %d%
if errorlevel 1 goto error
goto end

:jelly
E:\smvs_mcs\trunk\product_switcher.exe JELLY_PRODUCT %d%
if errorlevel 1 goto error
goto end

:can_outside
E:\smvs_mcs\trunk\product_switcher.exe CAN_OUTSIDE_PRODUCT %d%
if errorlevel 1 goto error
goto end

:nail
E:\smvs_mcs\trunk\product_switcher.exe NAIL_PRODUCT %d%
if errorlevel 1 goto error
goto end

:plug
E:\smvs_mcs\trunk\product_switcher.exe PLUG_PRODUCT %d%
if errorlevel 1 goto error
goto end

:rubbermat
E:\smvs_mcs\trunk\product_switcher.exe MAT_PRODUCT %d%
if errorlevel 1 goto error
goto end

:hdpe
E:\smvs_mcs\trunk\product_switcher.exe HDPE_PRODUCT %d%
if errorlevel 1 goto error
goto end

:BatteryShell
E:\smvs_mcs\trunk\product_switcher.exe BATTERY_PRODUCT %d%
if errorlevel 1 goto error
goto end

:Capacitor
E:\smvs_mcs\trunk\product_switcher.exe CAP_PRODUCT %d%
if errorlevel 1 goto error
goto end

:Bottle
E:\smvs_mcs\trunk\product_switcher.exe BOTTLE_PRODUCT %d%
if errorlevel 1 goto error
goto end

:error
echo 配置图检软件平台失败！请联系开发人员
pause
exit

:end

echo 配置图检软件平台成功!
pause
exit