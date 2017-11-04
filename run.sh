#PBS -l nodes=4:ppn=2
#PBS -m abe
#PBS -M matthew.stagoll@gmail.com
source /etc/bash.bashrc
cd $PBS_O_WORKDIR
mpirun main -l s -P 0.6 -p 2 -n 8 -t 1 -m 8192 -M 8192
