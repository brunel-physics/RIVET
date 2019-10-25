from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'dilepton_analysis_2016'
config.General.workArea = 'dilepton'
config.General.transferOutputs = True
config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'dilepton_fulldataset2016.py'
config.JobType.inputFiles = ['/afs/cern.ch/user/c/coldham/private/CMSSW_10_0_0/src/GeneratorInterface/RivetInterface/data/TZQ_DILEPTON_2016.plot']
config.JobType.sendExternalFolder = True 
config.JobType.outputFiles = ['TZQ.yoda'] 

config.Data.inputDataset = '/tZq_ll_4f_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'
config.Data.inputDBS = 'global'
config.Data.splitting = 'Automatic'
config.Data.outLFNDirBase = '/store/user/%s/' % (getUsernameFromSiteDB())
config.Data.publication = False
config.Data.outputDatasetTag = 'CRAB_dilepton_analysis_2016'

config.Site.storageSite = 'T2_UK_London_Brunel'
