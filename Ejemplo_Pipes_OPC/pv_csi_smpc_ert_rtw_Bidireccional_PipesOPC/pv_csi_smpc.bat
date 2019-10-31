set MATLAB=C:\Program Files\MATLAB\R2018a


call  "\\MarioAndres-PC\C$\Program Files\MATLAB\R2018a\bin\win64\checkMATLABRootForDriveMap.exe" "\\MarioAndres-PC\C$\Program Files\MATLAB\R2018a"  > mlEnv.txt
for /f %%a in (mlEnv.txt) do set "%%a"\n
cd .

if "%1"=="" ("C:\Program Files\MATLAB\R2018a\bin\win64\gmake" MATLAB_ROOT=%MATLAB_ROOT% ALT_MATLAB_ROOT=%ALT_MATLAB_ROOT% MATLAB_BIN=%MATLAB_BIN% ALT_MATLAB_BIN=%ALT_MATLAB_BIN%  -f pv_csi_smpc.mk all) else ("C:\Program Files\MATLAB\R2018a\bin\win64\gmake" MATLAB_ROOT=%MATLAB_ROOT% ALT_MATLAB_ROOT=%ALT_MATLAB_ROOT% MATLAB_BIN=%MATLAB_BIN% ALT_MATLAB_BIN=%ALT_MATLAB_BIN%  -f pv_csi_smpc.mk %1)
@if errorlevel 1 goto error_exit

exit /B 0

:error_exit
echo The make command returned an error of %errorlevel%
An_error_occurred_during_the_call_to_make
