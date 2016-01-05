#!/usr/bin/python

import os, subprocess, time, smtplib

time.sleep(60)

sender = 'xedaq@astro18.astronevis.columbia.edu'
receiver = ['mda2149@columbia.edu']
message = """From: xedaq <xedaq@astro18.astronevis.columbia.edu>
To: Matt <mda2149@columbia.edu>
Subject: Astro18 Restarted - Restarting smacd

Astro18 Restarted - Restarting smacd
"""

os.system('cd /home/xedaq/nerix/smac-0.3.0; ./smacd')

smtpObj = smtplib.SMTP('localhost')
smtpObj.sendmail(sender, receiver, message)

