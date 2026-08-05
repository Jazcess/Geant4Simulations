#!/bin/bash
#SBATCH --job-name=G4_EMERGENCY
#SBATCH --partition=devel          # The "Express Lane"
#SBATCH --nodes=1
#SBATCH --cpus-per-task=50          # 8 cores is easier to find than 32
#SBATCH --mem=8G                   # Less RAM = faster start
#SBATCH --time=02:00:00            # MANDATORY MAX: 2 hours
#SBATCH --output=%x-%j.out

# If you are part of a research project, uncomment the line below 
# and replace 'PROJECT_NAME' with your actual project account.
# #SBATCH --account=PROJECT_NAME

cd $SLURM_SUBMIT_DIR

echo "Starting high-priority job on $SLURM_CPUS_PER_TASK cores..."
./wcdSim run.mac