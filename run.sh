#PBS -l nodes=28:ppn=6
#PBS -m abe
#PBS -M matthew.stagoll@gmail.com
source /etc/bash.bashrc
cd $PBS_O_WORKDIR
mpirun main -l s -P 0.6 -p 2 -n 28 -t 12 -m 32764 -M 32764
