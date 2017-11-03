#PBS -l nodes=5:ppn=6
#PBS -m abe
#PBS -M matthew.stagoll@gmail.com
source /etc/bash.bashrc
cd $PBS_O_WORKDIR
mpirun main -l s -P 0.6 -p 2 -n 28 -t 6 -m 1024 -M 1024
