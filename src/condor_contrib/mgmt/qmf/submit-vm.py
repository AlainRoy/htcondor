#!/usr/bin/env python
# -*- coding: utf-8 -*-

from qmf.console import Session
from sys import exit, argv
import time

UNIVERSE = {"VANILLA": 5, "SCHEDULER": 7, "GRID": 9, "JAVA": 10, "PARALLEL": 11, "LOCAL": 12, "VM": 13}

__annotations__ = {"Requirements": "com.redhat.grid.Expression"}
ad = {"Cmd":			"QMF Submitted VM",
      "!!descriptors":		__annotations__,
      "JobUniverse":		UNIVERSE["VM"],
      "Iwd":			"/tmp",
      "Owner":			"nobody",
      "ShouldTransferFiles":	"NEVER",
      "JobVMType":		"kvm",
      "VMPARAM_Kvm_Kernel":	"included",
      "VMPARAM_Kvm_Disk":	"/tmp/RHEL5.img:hda:w",
      "JobVMMemory":		512,
      "Requirements":		"""((Arch == "X86_64") && (HasVM) && (VM_Type == "kvm") && (VM_AvailNum > 0) && (TotalDisk >= DiskUsage) && (HasFileTransfer)) && (TARGET.FileSystemDomain == MY.FileSystemDomain) && (TotalMemory >= 128) && (VM_Memory >= 128)"""
      }

quiet = False
url = "amqp://localhost:5672"

for arg in argv[1:]:
	if arg == '-q':
		quiet = True
	if "amqp://" in arg:
		url = arg

if not quiet:
	print ad;

try:
	session = Session()
	broker = session.addBroker(url)
	schedulers = session.getObjects(_class="scheduler", _package="com.redhat.grid")
except Exception, e:
	print "unable to access broker or scheduler"
	exit(1)

result = schedulers[0].SubmitJob(ad)

if result.status:
    print "Error submitting:", result.text
    session.delBroker(broker)
    exit(1)

if not quiet:
	print "Submitted:", result.Id
	print "waiting for Submission to propagate..."
time.sleep(10)

submissions = session.getObjects(_class="submission", _package="com.redhat.grid")
submission = reduce(lambda x,y: x or (y.Name == result.Id and y), submissions, False)
if not submission:
    print "Did not find submission... is the schedd or jobserver publishing submissions?"
    session.delBroker(broker)
    exit(1)

if not quiet:
	print "Properties:"
	for prop in submission.getProperties():
		print " ",prop[0],"=",prop[1]

print result.Id.rsplit('#')[1]

session.delBroker(broker)
