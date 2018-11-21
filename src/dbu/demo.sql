
CREATE DATABASE lhp_learndemo

create table template_table(template_id INTEGER NOT NULL,
							width INTEGER,
							height INTEGER, 
							duration INTEGER,
							usr_text_count INTEGER,
							usr_pic_count INTEGER,
							usr_video_count INTEGER,
							usr_audio_count INTEGER,
					        PRIMARY KEY (template_id));

INSERT INTO template_table(template_id,
						    width,
							height,
							duration,
							usr_text_count,
							usr_pic_count,
							usr_video_count,
							usr_audio_count) VALUES(110,1920,1080,4,0,0,1,0);
							
INSERT INTO template_table(template_id,
						    width,
							height,
							duration,
							usr_text_count,
							usr_pic_count,
							usr_video_count,
							usr_audio_count) VALUES(111,1920,1080,4,0,0,1,0);
INSERT INTO template_table(template_id,
						    width,
							height,
							duration,
							usr_text_count,
							usr_pic_count,
							usr_video_count,
							usr_audio_count) VALUES(112,1920,1080,4,0,0,1,0);
INSERT INTO template_table(template_id,
						    width,
							height,
							duration,
							usr_text_count,
							usr_pic_count,
							usr_video_count,
							usr_audio_count) VALUES(113,1920,1080,4,0,0,1,0);
INSERT INTO template_table(template_id,
						    width,
							height,
							duration,
							usr_text_count,
							usr_pic_count,
							usr_video_count,
							usr_audio_count) VALUES(114,1920,1080,4,0,0,1,0);
INSERT INTO template_table(template_id,
						    width,
							height,
							duration,
							usr_text_count,
							usr_pic_count,
							usr_video_count,
							usr_audio_count) VALUES(115,1920,1080,4,0,0,1,0);
INSERT INTO template_table(template_id,
						    width,
							height,
							duration,
							usr_text_count,
							usr_pic_count,
							usr_video_count,
							usr_audio_count) VALUES(116,1920,1080,4,0,0,1,0);
INSERT INTO template_table(template_id,
						    width,
							height,
							duration,
							usr_text_count,
							usr_pic_count,
							usr_video_count,
							usr_audio_count) VALUES(117,1920,1080,4,0,0,1,0);
INSERT INTO template_table(template_id,
						    width,
							height,
							duration,
							usr_text_count,
							usr_pic_count,
							usr_video_count,
							usr_audio_count) VALUES(118,1920,1080,4,0,0,1,0);
INSERT INTO template_table(template_id,
						    width,
							height,
							duration,
							usr_text_count,
							usr_pic_count,
							usr_video_count,
							usr_audio_count) VALUES(119,1920,1080,4,0,0,1,0);
							
INSERT INTO template_table(template_id,
						    width,
							height,
							duration,
							usr_text_count,
							usr_pic_count,
							usr_video_count,
							usr_audio_count) VALUES(120,1920,1080,4,0,1,0,0);
INSERT INTO template_table(template_id,
						    width,
							height,
							duration,
							usr_text_count,
							usr_pic_count,
							usr_video_count,
							usr_audio_count) VALUES(121,1920,1080,4,0,1,0,0);
INSERT INTO template_table(template_id,
						    width,
							height,
							duration,
							usr_text_count,
							usr_pic_count,
							usr_video_count,
							usr_audio_count) VALUES(122,1920,1080,4,0,1,0,0);
INSERT INTO template_table(template_id,
						    width,
							height,
							duration,
							usr_text_count,
							usr_pic_count,
							usr_video_count,
							usr_audio_count) VALUES(123,1920,1080,4,0,1,0,0);
INSERT INTO template_table(template_id,
						    width,
							height,
							duration,
							usr_text_count,
							usr_pic_count,
							usr_video_count,
							usr_audio_count) VALUES(124,1920,1080,4,0,1,0,0);
INSERT INTO template_table(template_id,
						    width,
							height,
							duration,
							usr_text_count,
							usr_pic_count,
							usr_video_count,
							usr_audio_count) VALUES(125,1920,1080,4,0,1,0,0);

							
SELECT * FROM template_table;

SELECT template_id FROM template_table where usr_video_count=1;
SELECT template_id ,width FROM template_table where usr_video_count=1;
							
							
					   
					   
					   
					   
					   