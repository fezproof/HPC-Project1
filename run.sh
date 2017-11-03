#PBS -l nodes=28:ppn=6
#PBS -m abe
#PBS -M matthew.stagoll@gmail.com
source /etc/bash.bashrc
cd $PBS_O_WORKDIR
mpirun main -l s -P 0.6 -p 2 -n 7 -t 6 -m 8192 -M 8192
