#PBS -l nodes=28:ppn=12
#PBS -m abe
#PBS -M matthew.stagoll@gmail.com
source /etc/bash.bashrc
cd $PBS_O_WORKDIR
mpirun main -l s -P 0.6 -p 2 -n 2 -t 1 -m 64 -M 2048
