from subprocess import call

#call(['./scripts/process.py', '-f', 'nerix-low-voltages-run_15.xml', 'nerix_151109_1727', '20'])
#call(['./scripts/process.py', '-f', 'nerix-low-voltages-run_15.xml', 'nerix_151109_1727', '20'])
#call(['./scripts/process.py', '-f', 'nerix-default-run_15.xml', 'nerix_151116_1525', '20'])

call(['./scripts/process.py', '-f', 'nerix-coin-long-window-run_15.xml', 'nerix_151029_1753', '20'])
call(['./scripts/process.py', '-f', 'nerix-coin-long-window-run_15.xml', 'nerix_151030_1036', '20'])
call(['./scripts/process.py', '-f', 'nerix-coin-long-window-run_15.xml', 'nerix_151031_1156', '20'])
call(['./scripts/process.py', '-f', 'nerix-coin-long-window-run_15.xml', 'nerix_151106_1627', '20'])
call(['./scripts/process.py', '-f', 'nerix-coin-run_15.xml', 'nerix_151113_1717', '20'])


