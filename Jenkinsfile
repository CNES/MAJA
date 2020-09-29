pipeline {
  agent {
    node { label 'docker' }
  }
  options {
    gitLabConnection('gitlab.cnes.fr')
    gitlabBuilds(builds: ['prepare', 'build'])
    skipDefaultCheckout true
  }
  environment {
    http_proxy = "http://surf.loc.cnes.fr:8050"
    https_proxy = "${env.http_proxy}"
    ftp_proxy = "${env.http_proxy}"
    no_proxy = "cnes.fr,sis.cnes.fr,gitlab.cnes.fr"
  }
  stages{
    stage("checkout") {
      steps {
          deleteDir()
          sh 'mkdir -p certs'
          sh 'cp /etc/pki/ca-trust/source/anchors/AC*.crt certs/'
          sh 'printenv'
          
          checkout scm: [
              $class: 'GitSCM',
              clearWorkspace: true,
              branches: [[name: "origin/${env.gitlabSourceBranch}"]],
              doGenerateSubmoduleConfigurations: false,
              extensions: [[$class : 'WipeWorkspace'],
                           [$class : 'SubmoduleOption',
                           disableSubmodules: false,
                           parentCredentials: false,
                           recursiveSubmodules: false,
                           references: '',
                           trackingSubmodules: false]],
              userRemoteConfigs: [[name: 'origin', 
                                   url: "${env.gitlabSourceRepoHttpUrl}", 
                                   credentialsId: "jenkins",
                                   ]],
          ]
      }
    }
    stage("prepare") {
      steps{
        updateGitlabCommitStatus name: 'prepare', state: 'running'
        //sh 'SuperBuild/CI/ci_launcher.sh xenial'
        sh 'SuperBuild/CI/ci_launcher.sh centos6'
        //sh 'SuperBuild/CI/ci_launcher.sh centos7'
      }
      post {
        failure { updateGitlabCommitStatus name: 'prepare', state: 'failed'}
        success { updateGitlabCommitStatus name: 'prepare', state: 'success'}
        always { archiveArtifacts artifacts: 'out/*/logs/**/*.log' }
      }
    }
    stage("build") {
      steps{
        updateGitlabCommitStatus name: 'build', state: 'running'
        //sh 'CI/ci_launcher.sh xenial'
        sh 'CI/ci_launcher.sh centos6'
        //sh 'CI/ci_launcher.sh centos7'
      }
      post {
        failure { updateGitlabCommitStatus name: 'build', state: 'failed'}
        success { updateGitlabCommitStatus name: 'build', state: 'success'}
        always {  archiveArtifacts artifacts: 'out/*/pkg/*.run, out/*/logs/build.log' }
      }
    }
    stage("deploy") {
      when {
        expression { return env.gitlabSourceBranch == "master"}
        expression { return env.gitlabSourceNamespace == "maja"}
        }
      steps{
        echo 'Deploy TODO'
      }
      post {
        failure { updateGitlabCommitStatus name: 'deploy', state: 'failed'}
        success { updateGitlabCommitStatus name: 'deploy', state: 'success'}
      }
    }
  }
}
