pipeline {
    agent {
        kubernetes {
            defaultContainer 'jenkins-slave'
            inheritFrom 'jenkins-slave-cmake'
            //yamlFile 'pod.yaml'
        }
    }
    stages {
        stage ('cmake configuration'){
            when {
                anyOf {
                    branch 'main'; branch pattern: 'PR-\\d+', comparator: 'REGEXP'
                }
            }
            steps {
                sh '''
                    tree
                    mkdir build
                '''
                dir('build'){
                    sh 'cmake ..'
                }
            }
        }
        stage ('cmake compile project'){
            when {
                anyOf {
                    branch 'main'; branch pattern: 'PR-\\d+', comparator: 'REGEXP'
                }
            }
            steps {
                dir('build'){
                    sh 'make jsondb'
                }
            }
        }
        stage ('cmake compile tests'){
            when {
                anyOf {
                    branch 'main'; branch pattern: 'PR-\\d+', comparator: 'REGEXP'
                }
            }
            steps {
                dir('build'){
                    sh 'make jsondbTests'
                }
            }
        }
        stage ('run tests'){
            when {
                anyOf {
                    branch 'main'; branch pattern: 'PR-\\d+', comparator: 'REGEXP'
                }
            }
            steps {
                dir('build'){
                    sh 'ctest -V'
                }
            }
        }
        stage ('report coverage'){
            when {
                anyOf {
                    branch 'main'; branch pattern: 'PR-\\d+', comparator: 'REGEXP'
                }
            }
            steps {
                dir('build'){
                    sh 'make coverage'
                }
            }
        }
        stage ('sonar-scanner'){
            steps {
                script {
                    sonnarPath = tool('sonar-scanner')
                }
                withSonarQubeEnv(installationName: 'mydevopsway') {
                    echo 'running sonar-scanner'
                    sh "${sonnarPath}/bin/sonar-scanner -Dsonar.qualitygate.wait=true"
                }
            }
        }
    }
}
