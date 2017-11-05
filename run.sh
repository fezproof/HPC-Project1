#PBS -l walltime=0:10:00,nodes=1:ppn=5
#PBS -m abe
#PBS -M matthew.stagoll@gmail.com
#PBS -e ./error.txt
#PBS -o ./output.csv
source /etc/bash.bashrc
cd $PBS_O_WORKDIR
mpirun main -l b -P 0.6 -p 2 -n 5 -t 3 -m 128 -M 128 -r 1
