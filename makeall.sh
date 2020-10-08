#*****************************************************************************
# makeall.sh
#
# Change History:
#  VER.   Author         DATE              Change Description
#  1.0    Qiwei Wu       May. 04, 2019     Initial Release
#  1.1    Qiwei Wu       Apr. 06, 2020     Improvement
#  1.2    Qiwei Wu       Oct. 05, 2020     Add PetaLinux build
#  1.3    Qiwei Wu       Oct. 07, 2020     Add depends
#*****************************************************************************
#!/bin/bash

PrjName='qwi16_threadled'
ChipType='xc7z020clg400-2'
FwBuild='0'
SwBuild='0'
PetalinuxBuild='0'
CreateNew='1'
IverilogCompile='0'
MemfsBuild='0'

FwBuild=$1
PetalinuxBuild=$2
CreateNew=$3

#*****************************************************************************
# Assert
#*****************************************************************************
if [[ $FwBuild -eq 0 ]] && [[ $PetalinuxBuild -eq 0 ]]; then
   echo "Error: Missed build component. Example to build all: \"$0 1 1 1\""
   exit 1
fi

#*****************************************************************************
# depends
#*****************************************************************************
if [ $GITHUB ]; then
   #       (0, 1)
   depends=('qwi_base' 'git clone https://github.com/qiweiii-git/qwi_base.git')
else
   depends=('qwi_base' 'git clone https://gitee.com/qiweiii-gitee/qwi_base.git')
fi

#*****************************************************************************
# Dir defines
#*****************************************************************************
OutpFileDir='bin'
DependFileDir='.depend'
MakeFileDir=$DependFileDir/qwi_base/make
# Fw related
BuidTclFileDir=$DependFileDir/qwi_base/build
# Sw related
SWTclFileDir=$DependFileDir/qwi_base/build

if [ ! -d $OutpFileDir ]; then
   mkdir $OutpFileDir
   echo "Info: $OutpFileDir created"
fi

#*****************************************************************************
# Get depends
#*****************************************************************************
if [ ! -d $DependFileDir ]; then
   mkdir $DependFileDir
   echo "Info: $DependFileDir created"
fi

dependLen=${#depends[*]}

if (( $dependLen > 0 )); then
   cd $DependFileDir
   for((i=0; i<dependLen; i=i+2))
   do
      if [ ! -d ${depends[i]} ]; then
         echo "Getting ${depends[i]}"
         ${depends[i+1]}
      fi
   done
   cd ../
   echo "Info: All depends got"
fi

chmod 0755 -R $DependFileDir

#*****************************************************************************
# Build FW
#*****************************************************************************
if [ $FwBuild -eq 1 ]; then
   ./$MakeFileDir/makeFw.sh $PrjName $ChipType $BuidTclFileDir $OutpFileDir 1
fi

#*****************************************************************************
# Build SW
#*****************************************************************************
if [ $SwBuild -eq 1 ]; then
   ./$MakeFileDir/makeSw.sh $PrjName $SWTclFileDir $OutpFileDir $MemfsBuild
fi

#*****************************************************************************
# Build PetaLinux
#*****************************************************************************
if [ $PetalinuxBuild -eq 1 ]; then
   ./$MakeFileDir/makePetaLinux.sh $PrjName $OutpFileDir $CreateNew
fi
