from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'dilepton_2017_analysis'
config.General.workArea = 'dilepton2017'
config.General.transferOutputs = True
config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'dilepton_fulldataset2017.py'
config.JobType.inputFiles = ['/afs/cern.ch/user/c/coldham/private/CMSSW_10_0_0/src/GeneratorInterface/RivetInterface/data/TZQ_DILEPTON_2017.plot']
config.JobType.sendExternalFolder = True 
config.JobType.outputFiles = ['TZQ.yoda'] 

config.Data.inputDataset = '/tZq_ll_4f_ckm_NLO_TuneCP5_PSweights_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v2/MINIAODSIM'
config.Data.inputDBS = 'global'
config.Data.splitting = 'Automatic'
config.Data.outLFNDirBase = '/store/user/%s/' % (getUsernameFromSiteDB())
config.Data.publication = False
config.Data.outputDatasetTag = 'CRAB_dilepton_2017_analysis'

config.Site.storageSite = 'T2_UK_London_Brunel'
