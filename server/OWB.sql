-- MySQL dump 10.13  Distrib 5.1.69, for debian-linux-gnu (x86_64)
--
-- Host: localhost    Database: OWB
-- ------------------------------------------------------
-- Server version	5.1.69-0ubuntu0.11.10.1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `Meeting`
--

DROP TABLE IF EXISTS `Meeting`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Meeting` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `MeetingID` varchar(50) CHARACTER SET latin1 DEFAULT NULL,
  `state` int(11) NOT NULL DEFAULT '0',
  `Port` int(11) DEFAULT '0',
  `DataRef` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4547 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Meeting`
--

LOCK TABLES `Meeting` WRITE;
/*!40000 ALTER TABLE `Meeting` DISABLE KEYS */;
INSERT INTO `Meeting` VALUES (4508,'4508',0,0,NULL),(4509,'4509',1,11250,33552416),(4510,'4510',0,0,NULL),(4511,'4511',0,0,NULL),(4512,'4512',0,0,NULL),(4513,'4513',0,0,NULL),(4514,'4514',0,0,NULL),(4515,'4515',0,0,NULL),(4516,'4516',0,0,NULL),(4517,'4517',0,0,NULL),(4518,'4518',0,0,NULL),(4519,'4519',0,0,NULL),(4520,'4520',0,0,NULL),(4521,'4521',0,0,NULL),(4522,'4522',0,0,NULL),(4523,'4523',0,0,NULL),(4524,'4524',0,0,NULL),(4525,'4525',0,0,NULL),(4526,'4526',0,0,NULL),(4527,'4527',0,0,NULL),(4528,'4528',0,0,NULL),(4529,'4529',0,0,NULL),(4530,'4530',0,0,NULL),(4531,'4531',0,0,NULL),(4532,'4532',0,0,NULL),(4533,'4533',1,11249,33312208),(4534,'4534',0,0,NULL),(4535,'4535',0,0,NULL),(4536,'4536',1,11250,14717232),(4537,'4537',1,11250,10548944),(4538,'4538',1,11250,25768928),(4539,'4539',1,11250,35569472),(4540,'4540',1,11250,24325888),(4541,'4541',1,11250,16159200),(4542,'4542',1,11250,140700310127456),(4543,'4543',0,0,NULL),(4544,'4544',1,11250,24243696),(4545,'4545',0,0,NULL),(4546,'4546',1,11250,35854032);
/*!40000 ALTER TABLE `Meeting` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `MeetingDocument`
--

DROP TABLE IF EXISTS `MeetingDocument`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `MeetingDocument` (
  `DocumentID` int(11) NOT NULL AUTO_INCREMENT,
  `MeetingID` varchar(50) CHARACTER SET latin1 NOT NULL,
  `path` varchar(100) CHARACTER SET latin1 NOT NULL,
  PRIMARY KEY (`DocumentID`)
) ENGINE=InnoDB AUTO_INCREMENT=7069 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `MeetingDocument`
--

LOCK TABLES `MeetingDocument` WRITE;
/*!40000 ALTER TABLE `MeetingDocument` DISABLE KEYS */;
/*!40000 ALTER TABLE `MeetingDocument` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `MeetingUser`
--

DROP TABLE IF EXISTS `MeetingUser`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `MeetingUser` (
  `MeetingID` varchar(50) CHARACTER SET latin1 NOT NULL,
  `UserID` varchar(100) CHARACTER SET latin1 NOT NULL,
  `State` int(11) NOT NULL,
  `HBTIme` bigint(20) NOT NULL,
  `id` int(11) NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1972 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `MeetingUser`
--

LOCK TABLES `MeetingUser` WRITE;
/*!40000 ALTER TABLE `MeetingUser` DISABLE KEYS */;
INSERT INTO `MeetingUser` VALUES ('4509','T',3,1368271910,1950),('4533','Y',3,1368272519,1951),('4533','Y',1,1368272519,1952),('4536','U',3,1368272620,1953),('4536','F',1,1368272616,1954),('4537','G',3,1368273067,1955),('4537','Dfas',1,1368273065,1956),('4538','Y',3,1368273318,1957),('4538','L',1,1368273323,1958),('4539','Y',3,1368273690,1959),('4539','D',1,1368273690,1960),('4540','I',3,1368274121,1961),('4540','F',1,1368274117,1962),('4541','Y',3,1368274344,1963),('4541','Fight',1,1368274456,1964),('4542','Gfffg',3,1368274583,1965),('4542','D',1,1368274587,1966),('4544','G',3,1368275033,1967),('4544','L',1,1368274901,1968),('4544','D',1,1368275031,1969),('4546','Grudge',3,1368275311,1970),('4546','D',1,1368275310,1971);
/*!40000 ALTER TABLE `MeetingUser` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2013-05-11 20:59:08