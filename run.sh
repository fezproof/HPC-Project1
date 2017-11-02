#PBS -l nodes=4:ppn=1
#PBS -m abe
#PBS -M matthew.stagoll@gmail.com
source /etc/bash.bashrc
cd $PBS_O_WORKDIR
mpirun main -l s -P 0.6 -p 2 -n 4 -t 1 -m 8 -M 8
