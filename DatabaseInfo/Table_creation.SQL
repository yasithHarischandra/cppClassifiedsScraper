--schema
DROP SCHEMA IF EXISTS classifieds CASCADE;
CREATE SCHEMA classifieds;

--Following queries are for creating the required tables
DROP TABLE IF EXISTS classifieds.vehicle_ad;
CREATE TABLE classifieds.vehicle_ad (
	vehicle_ad_id SERIAL PRIMARY KEY,
	vehicle_type_id INT NOT NULL,
	vehicle_maker_id INT NOT NULL,
	vehicle_model VARCHAR(50) NOT NULL,
	vehicle_ad_year INT,
	vehicle_ad_mileage INT,
	vehicle_ad_transmission VARCHAR(20),
	vehicle_ad_fuel_type VARCHAR(20),
	vehicle_ad_engine_capacity INT,
	vehicle_ad_start_type VARCHAR(20),
	vehicle_ad_price TEXT,
	vehicle_ad_price_num INT,
	vehicle_ad_ongoing_leasing BOOLEAN,
	vehicle_ad_url TEXT UNIQUE NOT NULL,
	vehicle_ad_date DATE NOT NULL,
	city_id INT NOT NULL,
	vehicle_ad_contactno VARCHAR(15),
	vehicle_ad_options TEXT,
	vehicle_ad_details TEXT
);

DROP TABLE IF EXISTS classifieds.vehicle_type;
CREATE TABLE classifieds.vehicle_type (
	vehicle_type_id SERIAL PRIMARY KEY,
	vehicle_type VARCHAR(20) UNIQUE NOT NULL
);

DROP TABLE IF EXISTS classifieds.vehicle_maker;
CREATE TABLE classifieds.vehicle_maker (
	vehicle_maker_id SERIAL PRIMARY KEY,
	vehicle_maker VARCHAR(50) UNIQUE NOT NULL,
	vehicle_maker_country VARCHAR(50)
);

DROP TABLE IF EXISTS classifieds.district;
CREATE TABLE classifieds.district (
	district_id SERIAL PRIMARY KEY,
	district VARCHAR(30) UNIQUE NOT NULL
);

DROP TABLE IF EXISTS classifieds.city;
CREATE TABLE classifieds.city (
	city_id SERIAL PRIMARY KEY,
	city VARCHAR(50) UNIQUE NOT NULL,
	district_id INT
);

--Folowing queries are for foreign keys

ALTER TABLE classifieds.city
ADD CONSTRAINT constraint_fk_district
FOREIGN KEY (district_id)
REFERENCES classifieds.district(district_id)
ON DELETE CASCADE;

ALTER TABLE classifieds.vehicle_ad
ADD CONSTRAINT constraint_fk_vehicle_type
FOREIGN KEY (vehicle_type_id)
REFERENCES classifieds.vehicle_type(vehicle_type_id)
ON DELETE CASCADE,
ADD CONSTRAINT constraint_fk_vehicle_maker
FOREIGN KEY (vehicle_maker_id)
REFERENCES classifieds.vehicle_maker(vehicle_maker_id)
ON DELETE CASCADE,
ADD CONSTRAINT constraint_fk_city
FOREIGN KEY (city_id)
REFERENCES classifieds.city(city_id)
ON DELETE CASCADE;
